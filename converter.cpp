#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
#ifdef _WIN32
#include <windows.h>
#endif

// 类型表达式
struct TypeExpr
{
    std::string name;
    std::vector<TypeExpr> arguments;
    bool isTypeVar = false;

    TypeExpr() = default;
    TypeExpr(std::string n, bool isVar = false) : name(std::move(n)), isTypeVar(isVar) {}
};

struct PatternNode
{
    enum class Kind
    {
        Wildcard, // 对应 thy 中的 _
        Variable, // 对应 thy 的一个变量
        Constructor // 对应 thy 的构造子模式
    };
    Kind kind = Kind::Variable; // 默认为变量
    std::string name;
    std::vector<PatternNode> children; // 当 kind == Constructor 时有效，保存每个子模式
};

struct Expression
{
    enum class Kind
    {
        Variable,
        ConstructorCall,
        FunctionCall,
        BinaryOperator,
        IfThenElse,
        LetBinding,
        CaseExpression,
        Unsupported
    };
    Kind kind = Kind::Unsupported;
    std::string name;
    std::vector<Expression> arguments; // 子表达式列表
    std::string raw;

    // Let 绑定相关
    bool hasLet = false;
    std::string letVar;
    int letValueIndex = -1;
    int letBodyIndex = -1;

    // case 相关
    bool hasCase = false;
    int caseScrutineeIndex = -1; // 存储 case 表达式中被匹配的值在 arguments 向量中的索引
    
    // 每一个 case 分支
    struct CaseAlternative
    {
        PatternNode pattern;
        int bodyIndex = -1;
    };
    // 所有分支列表
    std::vector<CaseAlternative> caseAlternatives;

    static Expression makeUnsupported(const std::string &text)
    {
        Expression expr;
        expr.kind = Kind::Unsupported;
        expr.raw = text;
        return expr;
    }
};

struct Constructor
{
    std::string name;
    // 该构造子接受的字段类型列表
    // eg. Cons "'a" "'a mylist"
    // ->  argumentTypes = [TypeExpr("'a"), TypeExpr("'a mylist")]
    std::vector<TypeExpr> argumentTypes;
};

// datatype 声明 DatatypeDecl

// eg.
// Isabelle:
// datatype mynat = Zero | Suc mynat
// datatype 'a mylist = Nil | Cons "'a" "'a mylist"

// 解析后的 DatatypeDecl 对象：

// 1. name: "mynat"
//    typeParams: []                          // 无类型参数
//    constructors: [
//      { name: "Zero",  argumentTypes: [] },
//      { name: "Suc",   argumentTypes: [mynat] }
//    ]

// 2. name: "mylist"
//    typeParams: ["a"]                       // 一个类型参数
//    constructors: [
//      { name: "Nil",   argumentTypes: [] },
//      { name: "Cons",  argumentTypes: ['a, 'a mylist] }
//    ]

struct DatatypeDecl
{
    std::string name;
    std::vector<std::string> typeParams;
    std::vector<Constructor> constructors;
};

// 一个子句，也就是一个模式匹配分支

// eg.
// Isabelle:
// fun add :: "mynat⇒mynat⇒mynat" where
//   "add Zero n = n"
// | "add (Suc m) n = Suc (add m n)"

// 解析后的两个 FunctionClause 对象：

// 1. patterns: [Variable("Zero"), Variable("n")]
//    body: "n"
//    bodyExpr: Expression(kind=Variable, name="n")

// 2. patterns: [Constructor("Suc", [Variable("m")]), Variable("n")]
//    body: "Suc (add m n)"
//    bodyExpr: Expression(kind=ConstructorCall, name="Suc", arguments=[...])

struct FunctionClause
{
    std::vector<PatternNode> patterns; // 该子句的模式列表
    std::string body; // 子句等号右边的原始文本
    Expression bodyExpr; // 解析后的表达式AST
};

// 一个函数定义
struct FunDecl
{
    std::string name; // 函数名 add
    std::string signature; // 原始类型签名文本 "mynat⇒mynat⇒mynat"
    std::vector<std::string> typeVars; // 函数用到的类型变量列表
    std::vector<TypeExpr> argumentTypes; // 从 signature 解析得到的参数类型列表
    TypeExpr returnType; // 从 signature 解析得到的返回值类型
    std::vector<FunctionClause> clauses; // 函数的所有子句，组成一个列表
};

// 表示 thy 文件解析后的AST
struct ModuleAst
{
    std::string theoryName; // thy 名称
    std::vector<DatatypeDecl> datatypes; // 所有 datatype 声明，一个列表
    std::vector<FunDecl> functions; // 所有 fun 定义，一个列表
};

// 去掉字符串两端的空白字符
static std::string trim(const std::string &value)
{
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
    {
        start++;
    }
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
    {
        end--;
    }
    return value.substr(start, end - start);
}

// 按指定分隔符将字符串分割成多个子串
// 然后放入vector返回
static std::vector<std::string> split(const std::string &value, char delimiter)
{
    std::vector<std::string> parts;
    std::string item;
    std::stringstream ss(value);
    while (std::getline(ss, item, delimiter))
    {
        parts.push_back(trim(item));
    }
    return parts;
}

// cout 并 cin
static std::string promptLine(const std::string &message)
{
    std::cout << message;
    std::string line;
    std::getline(std::cin, line);
    return trim(line);
}

// 用于正常输入输出 => 等符号，设置控制台为 UTF-8 编码
static void setupConsole()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

// 检查text是否以keyword开头，且后面是空格或结尾
// 用于识别 fun、datatype 等关键字
static bool startsWithKeyword(const std::string &text, const std::string &keyword)
{
    // 如果 text 不以 keyword 开头，直接返回 false
    if (text.rfind(keyword, 0) != 0)
    {
        return false;
    }
    // 如果 text 长度等于 keyword 长度，说明完全匹配，返回 true
    if (text.size() == keyword.size())
    {
        return true;
    }
    // 取出 keyword 后的下一个字符，检查是否为空白字符
    char next = text[keyword.size()];
    return std::isspace(static_cast<unsigned char>(next));
}

// 去掉字符串两端的引号
static std::string stripQuotes(const std::string &value)
{
    if (value.size() >= 2 && ((value.front() == '"' && value.back() == '"') || (value.front() == '\'' && value.back() == '\'')))
    {
        return value.substr(1, value.size() - 2);
    }
    return value;
}

// 替换字符串中所有出现的 from 为 to，用于将⇒变为C++可用的->
static std::string replaceAll(std::string text, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    while ((pos = text.find(from, pos)) != std::string::npos)
    {
        text.replace(pos, from.size(), to);
        pos += to.size();
    }
    return text;
}

// 检查括号是否配对平衡
static bool hasBalancedParens(const std::string &text)
{
    int depth = 0;
    for (char ch : text)
    {
        if (ch == '(')
        {
            depth++;
        }
        else if (ch == ')')
        {
            depth--;
            if (depth < 0)
            {
                return false;
            }
        }
    }
    return depth == 0;
}

// 按顶层的 '->' 分割类型签名字符串
// 遍历时维护括号深度，确保只在深度为零时分割
// eg. "mynat -> mynat -> mynat" -> ["mynat", "mynat", "mynat"]
static std::vector<std::string> splitByArrow(const std::string &text)
{
    std::vector<std::string> parts;
    std::string current;
    int depth = 0;
    for (size_t i = 0; i < text.size(); ++i)
    {
        if (text[i] == '(')
        {
            depth++;
            current.push_back(text[i]);
        }
        else if (text[i] == ')')
        {
            depth--;
            current.push_back(text[i]);
        }
        else if (text[i] == '-' && i + 1 < text.size() && text[i + 1] == '>' && depth == 0)
        {
            parts.push_back(trim(current));
            current.clear();
            ++i;
        }
        else
        {
            current.push_back(text[i]);
        }
    }
    if (!current.empty())
    {
        parts.push_back(trim(current));
    }
    return parts;
}

// 按空白字符分割字符串，但尊重括号内的内容不被分割
// eg. splitTokensRespectingParens("Cons x (y z)") → ["Cons", "x", "(y z)"]
static std::vector<std::string> splitTokensRespectingParens(const std::string &text)
{
    std::vector<std::string> tokens;
    std::string current;
    int depth = 0;
    for (size_t i = 0; i < text.size(); ++i)
    {
        char ch = text[i];
        if (std::isspace(static_cast<unsigned char>(ch)) && depth == 0)
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
        }
        else
        {
            if (ch == '(')
            {
                depth++;
            }
            else if (ch == ')')
            {
                depth--;
            }
            current.push_back(ch);
        }
    }
    if (!current.empty())
    {
        tokens.push_back(current);
    }
    return tokens;
}

// 清理类型标记，去掉多余的空白和类型约束
// eg. "'a :: type" -> "'a"
static std::string sanitizeTypeToken(std::string token)
{
    token = trim(stripQuotes(token));
    auto constraintPos = token.find("::");
    if (constraintPos != std::string::npos)
    {
        token = token.substr(0, constraintPos);
    }
    return trim(token);
}

// C++ 保留字集合
static const std::set<std::string> cppKeywords = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
    "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
    "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected",
    "public", "register", "reinterpret_cast", "requires", "return", "short",
    "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
    "switch", "template", "this", "thread_local", "throw", "true", "try",
    "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual",
    "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
};

// 与 std 命名空间冲突的常见名称
static const std::set<std::string> stdConflicts = {
    "less", "greater", "equal_to", "not_equal_to", "less_equal", "greater_equal",
    "plus", "minus", "multiplies", "divides", "modulus", "negate",
    "logical_and", "logical_or", "logical_not",
    "bit_and", "bit_or", "bit_xor", "bit_not",
    "find", "count", "sort", "copy", "move", "swap", "min", "max"
};

// 将可能与 C++ 关键字或 std 冲突的标识符重命名
static std::string sanitizeIdentifier(const std::string &name)
{
    if (cppKeywords.count(name) || stdConflicts.count(name))
    {
        return name + "_";
    }
    return name;
}

static TypeExpr parseTypeTerm(const std::string &text);

// 解析一个原子类型的标记
// eg. "'a" -> TypeExpr(name="a", isTypeVar=true)
//  "mynat" -> TypeExpr(name="mynat", isTypeVar=false)
static TypeExpr parseAtomicType(const std::string &token)
{
    std::string cleaned = sanitizeTypeToken(token);
    if (cleaned.empty())
    {
        return TypeExpr("void");
    }
    if (cleaned.front() == '\'')
    {
        return TypeExpr(cleaned.substr(1), true);
    }
    return TypeExpr(cleaned, false);
}

// 不是很懂
// 解析一个完整的类型表达式
// "'a list" → base=TypeExpr("list")，arguments=[TypeExpr("a", true)]
// "mynat" → TypeExpr("mynat", false)
// "(int)" → 去括号后 → TypeExpr("int")
static TypeExpr parseTypeTerm(const std::string &text)
{
    std::string cleaned = trim(text);
    if (cleaned.empty())
    {
        return TypeExpr("void");
    }
    while (cleaned.size() >= 2 && cleaned.front() == '(' && cleaned.back() == ')' && hasBalancedParens(cleaned.substr(1, cleaned.size() - 2)))
    {
        cleaned = trim(cleaned.substr(1, cleaned.size() - 2));
    }
    auto tokens = splitTokensRespectingParens(cleaned);
    if (tokens.empty())
    {
        return TypeExpr(cleaned);
    }
    if (tokens.size() == 1)
    {
        return parseAtomicType(tokens[0]);
    }
    TypeExpr base = parseTypeTerm(tokens.back());
    std::string prefix;
    for (size_t i = 0; i + 1 < tokens.size(); ++i)
    {
        if (!prefix.empty())
        {
            prefix.push_back(' ');
        }
        prefix += tokens[i];
    }
    base.arguments.push_back(parseTypeTerm(prefix));
    return base;
}

// 收集类型表达式中出现的类型变量，按出现顺序存入 order 向量
// 用于生成template<typename T0>
static void collectTypeVarsOrdered(const TypeExpr &expr, std::vector<std::string> &order, std::set<std::string> &seen)
{
    if (expr.isTypeVar && !seen.count(expr.name))
    {
        order.push_back(expr.name);
        seen.insert(expr.name);
    }
    for (const auto &arg : expr.arguments)
    {
        collectTypeVarsOrdered(arg, order, seen);
    }
}

// 解析后的函数签名，提取参数类型、返回类型和类型变量
struct ParsedSignature
{
    std::vector<TypeExpr> args;
    TypeExpr result;
    std::vector<std::string> typeVars;
};

static ParsedSignature parseSignature(const std::string &signatureText)
{
    ParsedSignature parsed;
    // 规范化，统一箭头
    std::string normalized = stripQuotes(trim(signatureText));
    normalized = replaceAll(normalized, "\\<Rightarrow>", "->");
    normalized = replaceAll(normalized, "\xe2\x87\x92", "->"); // unicode ⇒
    normalized = replaceAll(normalized, "⇒", "->");

    // 按箭头分割成类型片段
    auto parts = splitByArrow(normalized);
    if (parts.empty())
    {
        parsed.result = TypeExpr("void");
        return parsed;
    }
    if (parts.size() == 1)
    {
        // 无参函数，只有返回值的类型
        parsed.result = parseTypeTerm(parts.back());
    }
    else
    {
        // 多参函数，前n - 1个是参数类型，最后一个是返回值类型
        for (size_t i = 0; i + 1 < parts.size(); ++i)
        {
            parsed.args.push_back(parseTypeTerm(parts[i]));
        }
        parsed.result = parseTypeTerm(parts.back());
    }

    // 收集所有类型变量
    std::set<std::string> seen;
    for (const auto &arg : parsed.args)
    {
        collectTypeVarsOrdered(arg, parsed.typeVars, seen);
    }
    collectTypeVarsOrdered(parsed.result, parsed.typeVars, seen);
    return parsed;
}

// 将 Isabelle 类型变量名（如 "a", "b"）映射到 C++ 模板参数名（T0、T1、...）
// eg. ['a, 'b] -> { 'a: T0, 'b: T1 }
// 作用：在 render Type() 中查表，将 TypeExpr 中的类型变量替换为模板参数名
static std::map<std::string, std::string> buildTypeVarMap(const std::vector<std::string> &vars)
{
    std::map<std::string, std::string> mapping;
    for (size_t i = 0; i < vars.size(); ++i)
    {
        std::string cppName = "T" + std::to_string(i);
        mapping[vars[i]] = cppName;
    }
    return mapping;
}

// 规范化类型参数列表，去掉引号和重复项
// eg. ["'a", "'b", "'a"] -> ["a", "b"] （来自 datatype 'a 'b 'a tree）
static std::vector<std::string> normalizeTypeParams(const std::vector<std::string> &params)
{
    std::vector<std::string> normalized;
    std::set<std::string> seen;
    for (const auto &name : params)
    {
        std::string cleaned = name;
        // 去掉前面的单引号（'a → a）
        if (!cleaned.empty() && cleaned.front() == '\'')
        {
            cleaned = cleaned.substr(1);
        }
        // 跳过空字符串或重复的类型参数
        if (cleaned.empty() || seen.count(cleaned))
        {
            continue;
        }
        normalized.push_back(cleaned);
        seen.insert(cleaned);
    }
    return normalized;
}

// 将 TypeExpr 树转换为 C++ 类型字符串。
static std::string renderType(const TypeExpr &expr, const std::map<std::string, std::string> &varMap)
{
    // 处理类型变量（'a 'b 'c这种），查表替换为模板参数名 
    if (expr.isTypeVar)
    {
        auto it = varMap.find(expr.name);
        if (it != varMap.end())
        {
            return it->second; // "a" → "T0"
        }
        return expr.name;
    }
    // 处理普通类型（无参数） mynat bool
    if (expr.arguments.empty())
    {
        return expr.name;
    }
    // 处理带类型参数的泛型类型
    // 渲染后（假设 varMap={"a"->"T0", "b"->"T1"}）：
    // - 'a list        -> "list<T0>"
    // - int option     -> "option<int>"
    // - ('a, 'b) tree  -> "tree<T0, T1>"
    // - ('a list) option -> "option<list<T0>>"
    std::string result = expr.name;
    result += "<";
    for (size_t i = 0; i < expr.arguments.size(); ++i)
    {
        result += renderType(expr.arguments[i], varMap);
        if (i + 1 < expr.arguments.size())
        {
            result += ", ";
        }
    }
    result += ">";
    return result;
}

// 将构造子声明文本（"xxx xxx xxx"这种双引号括起来的）分割成 token 列表，特殊处理引号中的内容
// eg. "\"Cons 'a 'a mylist\"" -> ["Cons", "'a", "'a mylist"]
static std::vector<std::string> tokenizeConstructorText(const std::string &text)
{
    std::vector<std::string> tokens;
    std::string current;
    bool inQuotes = false;
    for (char ch : text)
    {
        if (ch == '"')
        {
            inQuotes = !inQuotes;
            if (!inQuotes)
            {
                tokens.push_back(trim(current));
                current.clear();
            }
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(ch)) && !inQuotes)
        {
            if (!current.empty())
            {
                tokens.push_back(trim(current));
                current.clear();
            }
        }
        else
        {
            current.push_back(ch);
        }
    }
    if (!current.empty())
    {
        tokens.push_back(trim(current));
    }
    return tokens;
}

// 递归检查类型表达式 TypeExpr 中是否使用了指定的递归 datatype
// 用于判断一个字段类型是否涉及递归，是否需要用 shared_ptr 包装
// eg. 对于 datatype "mylist"：
// typeExprUsesDatatype(TypeExpr("list", args=[TypeExpr("mynat")]), "mynat")
//   → true （list 的参数是 mynat）
// typeExprUsesDatatype(TypeExpr("list", args=[TypeExpr("a", isTypeVar=true)]), "mylist")
//   → false （list 的参数是类型变量，不是 mylist）
// typeExprUsesDatatype(TypeExpr("mylist", args=[TypeExpr("mylist")]), "mylist")
//   → true （递归情况）
static bool typeExprUsesDatatype(const TypeExpr &expr, const std::string &datatypeName)
{
    if (expr.name == datatypeName)
    {
        return true;
    }
    for (const auto &child : expr.arguments)
    {
        if (typeExprUsesDatatype(child, datatypeName))
        {
            return true;
        }
    }
    return false;
}

// 渲染 datatype 构造子字段的类型
// 如果字段类型涉及递归 datatype，则用 shared_ptr 包装
// eg. 对于 datatype "mylist"：
// renderDatatypeFieldType(TypeExpr("mynat"), varMap, "mylist")
//   → "mynat" （非递归类型，直接返回）
// renderDatatypeFieldType(TypeExpr("mylist", args=[TypeExpr("mynat")]), varMap, "mylist")
//   → "std::shared_ptr<mylist<mynat>>" （递归类型，包装 shared_ptr）
static std::string renderDatatypeFieldType(const TypeExpr &expr, const std::map<std::string, std::string> &varMap, const std::string &datatypeName)
{
    std::string base = renderType(expr, varMap);
    if (typeExprUsesDatatype(expr, datatypeName))
    {
        return "std::shared_ptr<" + base + ">";
    }
    return base;
}

// 去掉最外层的括号，如果括号配对且包裹整个字符串，则返回去括号后的内容
// 否则返回 std::nullopt
// eg. "(a b c)" -> "a b c"
//      "(a (b c))" -> "a (b c)"
static std::optional<std::string> stripOuterParens(const std::string &text)
{
    std::string cleaned = trim(text);
    if (cleaned.size() < 2 || cleaned.front() != '(' || cleaned.back() != ')')
    {
        return std::nullopt;
    }
    int depth = 0;
    for (size_t i = 0; i < cleaned.size(); ++i)
    {
        char ch = cleaned[i];
        if (ch == '(')
        {
            depth++;
        }
        else if (ch == ')')
        {
            depth--;
            if (depth == 0 && i != cleaned.size() - 1)
            {
                return std::nullopt;
            }
        }
    }
    if (depth != 0)
    {
        return std::nullopt;
    }
    return trim(cleaned.substr(1, cleaned.size() - 2));
}

// 按顶层的指定分隔符分割字符串，忽略括号内的分隔符，维护括号深度
// eg. splitTopLevel("a b (c d) e", ' ') -> ["a", "b", "(c d)", "e"]
static std::vector<std::string> splitTopLevel(const std::string &text, char delimiter)
{
    std::vector<std::string> parts;
    std::string current;
    int depth = 0;
    for (size_t i = 0; i < text.size(); ++i)
    {
        char ch = text[i];
        if (ch == '(')
        {
            depth++;
        }
        else if (ch == ')')
        {
            depth--;
        }
        if (depth == 0 && ch == delimiter)
        {
            if (!current.empty())
            {
                parts.push_back(trim(current));
                current.clear();
            }
            continue;
        }
        current.push_back(ch);
    }
    if (!current.empty())
    {
        parts.push_back(trim(current));
    }
    return parts;
}

// 递归替换 TypeExpr 中的变量类型
// eg. substituteTypeVars(TypeExpr("list", args=[TypeExpr("a", isTypeVar=true)]), { "a": TypeExpr("mynat") })
//   → TypeExpr("list", args=[TypeExpr("mynat")])
static TypeExpr substituteTypeVars(const TypeExpr &expr, const std::map<std::string, TypeExpr> &substitution)
{
    if (expr.isTypeVar)
    {
        auto it = substitution.find(expr.name);
        if (it != substitution.end())
        {
            return it->second;
        }
        return expr;
    }
    TypeExpr result = expr;
    for (size_t i = 0; i < result.arguments.size(); ++i)
    {
        result.arguments[i] = substituteTypeVars(result.arguments[i], substitution);
    }
    return result;
}

// 根据 datatype 的泛型实例构建类型变量替换表

// DatatypeDecl: mylist (typeParams=["'a"])
// instance: mylist<int>  (TypeExpr("mylist", args=[TypeExpr("int")]))

// buildDatatypeSubstitution(instance, decl):
//   → key = "'a" → 去单引号 → "a"
//   → substitution["a"] = TypeExpr("int")
//   → 返回 {"a" → TypeExpr("int")}

// 用于后续替换 Cons 字段中的 "'a"
// Cons 的字段类型原本：['a, 'a mylist]
// 替换后：[int, mylist<int>]
static std::map<std::string, TypeExpr> buildDatatypeSubstitution(const TypeExpr &instance, const DatatypeDecl &decl)
{
    std::map<std::string, TypeExpr> substitution;
    for (size_t idx = 0; idx < decl.typeParams.size(); ++idx)
    {
        if (idx >= instance.arguments.size())
        {
            break;
        }
        std::string key = decl.typeParams[idx];
        if (!key.empty() && key.front() == '\'')
        {
            key = key.substr(1);
        }
        substitution[key] = instance.arguments[idx];
    }
    return substitution;
}

// 按空白分割字符串，保持括号内容作为整体
static std::vector<std::string> splitArgumentsRespectingParens(const std::string &text)
{
    std::vector<std::string> args;
    std::string current;
    int depth = 0;
    for (size_t i = 0; i < text.size(); ++i)
    {
        char ch = text[i];
        if (std::isspace(static_cast<unsigned char>(ch)) && depth == 0)
        {
            if (!current.empty())
            {
                args.push_back(trim(current));
                current.clear();
            }
        }
        else
        {
            if (ch == '(')
            {
                depth++;
            }
            else if (ch == ')')
            {
                depth--;
            }
            current.push_back(ch);
        }
    }
    if (!current.empty())
    {
        args.push_back(trim(current));
    }
    return args;
}

class Parser
{
public:
    explicit Parser(const std::string &path) : filePath(path) {}

    ModuleAst parse()
    {
        ModuleAst result;
        std::ifstream input(filePath);
        if (!input.is_open())
        {
            throw std::runtime_error("failed to open file: " + filePath);
        }

        std::string line;
        while (std::getline(input, line))
        {
            std::string clean = trim(line);

            // 识别 theory 行
            if (clean.rfind("theory", 0) == 0)
            {
                auto parts = split(clean, ' ');
                if (parts.size() > 1)
                {
                    result.theoryName = parts[1]; // 提取 theory 名称
                }
            }
            // 识别 datatype 行
            else if (clean.rfind("datatype", 0) == 0)
            {
                DatatypeDecl decl = parseDatatype(clean);
                result.datatypes.push_back(decl);
            }
            // 识别 fun 行
            else if (clean.rfind("fun", 0) == 0)
            {
                // 解析函数定义
                FunDecl fun = parseFunction(clean, input);
                result.functions.push_back(fun);
            }
        }

        if (result.theoryName.empty())
        {
            result.theoryName = "anonymous_theory";
        }
        return result;
    }

private:
    // thy 文件路径
    std::string filePath; 
    // 已知的构造子名称集合
    // 用于判断一个token是否为构造子
    std::set<std::string> knownConstructors; 
    // 构造子名称到所属 datatype 名称的映射
    std::map<std::string, std::string> constructorToDatatype; 

    DatatypeDecl parseDatatype(const std::string &line)
    {
        DatatypeDecl decl;

        // 按 '=' 分割，左边是类型参数和名称，右边是构造子声明
        auto pieces = split(line.substr(std::string("datatype").size()), '=');
        if (pieces.empty())
        {
            return decl;
        }

        // 解析左侧的类型参数和名称
        std::string header = trim(pieces[0]);
        auto headerParts = split(header, ' ');

        // 收集类型参数和 datatype 名称
        for (const auto &token : headerParts)
        {
            if (!token.empty() && token[0] == '\'')
            {
                decl.typeParams.push_back(token); // 以单引号开头的是类型参数
            }
            else if (decl.name.empty())
            {
                decl.name = token; // 第一个非类型参数的 token 是 datatype 名称
            }
        }

        // 重新组合构造子声明文本
        // 因为用 '=' 分割可能会破坏某些构造子的声明（如果构造子中含有 '='），
        // 所以要重新拼接
        std::string constructorsChunk;
        for (size_t i = 1; i < pieces.size(); ++i)
        {
            if (!constructorsChunk.empty())
            {
                constructorsChunk += '=';
            }
            constructorsChunk += pieces[i];
        }
        if (constructorsChunk.empty())
        {
            if (pieces.size() == 1 && line.find('=') != std::string::npos)
            {
                constructorsChunk = trim(line.substr(line.find('=') + 1));
            }
        }

        // 按 '|' 分割构造子声明
        auto constructorParts = split(constructorsChunk, '|');
        for (const auto &constructorText : constructorParts)
        {
            // tokenizeConstructorText 分割构造子声明中的 token
            auto tokens = tokenizeConstructorText(constructorText);
            if (tokens.empty())
            {
                continue;
            }
            Constructor ctor;
            ctor.name = tokens[0]; // 第一个 token 是构造子名称

            // 记录已知构造子和所属 datatype
            knownConstructors.insert(ctor.name);
            constructorToDatatype[ctor.name] = decl.name;

            // 解析剩余 token 为构造子的字段类型
            for (size_t idx = 1; idx < tokens.size(); ++idx)
            {
                std::string argText = stripQuotes(tokens[idx]);
                if (argText.empty())
                {
                    continue;
                }
                // 用 parseTypeTerm 解析字段类型
                ctor.argumentTypes.push_back(parseTypeTerm(argText));
            }
            decl.constructors.push_back(ctor);
        }
        return decl;
    }

    FunDecl parseFunction(const std::string &firstLine, std::ifstream &input)
    {
        FunDecl fun;
        // 解析函数名称和签名
        std::string clean = trim(firstLine.substr(std::string("fun").size()));
        auto wherePos = clean.find("where");
        std::string beforeWhere = wherePos == std::string::npos ? clean : trim(clean.substr(0, wherePos));

        auto parts = split(beforeWhere, ' ');
        if (!parts.empty())
        {
            fun.name = parts[0];
        }
        auto signaturePos = beforeWhere.find("::");
        if (signaturePos != std::string::npos)
        {
            fun.signature = trim(beforeWhere.substr(signaturePos + 2));
            ParsedSignature parsed = parseSignature(fun.signature);
            fun.argumentTypes = parsed.args;
            fun.returnType = parsed.result;
            fun.typeVars = parsed.typeVars;
        }

        // 解析函数所有子句
        std::string line;
        while (std::getline(input, line))
        {
            std::string trimmed = trim(line);
            if (trimmed.empty())
            {
                break; // 空行表示函数定义结束
            }
            
            // 处理行尾的 '|'
            bool hasTrailingBar = false;
            if (!trimmed.empty() && trimmed.back() == '|')
            {
                hasTrailingBar = true;
                trimmed = trim(trimmed.substr(0, trimmed.size() - 1));
            }
            // 处理行首的 '|' 或 '"|'
            bool startsWithQuote = false;
            if (!trimmed.empty() && trimmed.front() == '|')
            {
                std::string afterBar = trim(trimmed.substr(1));
                if (!afterBar.empty() && afterBar.front() == '"')
                {
                    trimmed = afterBar;
                    startsWithQuote = true;
                }
            }

            // 处理行首的 '"'
            if (!startsWithQuote)
            {
                startsWithQuote = !trimmed.empty() && trimmed.front() == '"';
                if (startsWithQuote)
                {
                    trimmed = trimmed.substr(1);
                }
            }

            // 处理行尾的 '"'
            if (!trimmed.empty() && trimmed.back() == '"')
            {
                trimmed = trimmed.substr(0, trimmed.size() - 1);
            }
            trimmed = trim(trimmed);
            if (trimmed.empty())
            {
                continue;
            }

            // 若行不以引号开头且已有子句，说明这行是前一个子句的延续
            if (!startsWithQuote && !fun.clauses.empty())
            {
                FunctionClause &previous = fun.clauses.back();
                previous.body += " " + trimmed;
                previous.body = trim(previous.body);
                previous.bodyExpr = parseExpression(previous.body);
                continue;
            }

            // 否则，这是一个新的子句
            FunctionClause clause = parseClause(trimmed, fun.name);
            fun.clauses.push_back(clause);
        }
        return fun;
    }

    // 把一条 Isabelle 风格的函数子句文本（例如 "add Zero n = n"）解析成 FunctionClause
    FunctionClause parseClause(const std::string &text, const std::string &funName)
    {
        auto eq = text.find('=');
        FunctionClause clause;
        if (eq == std::string::npos)
        {
            clause.body = trim(text);
            return clause;
        }
        std::string left = trim(text.substr(0, eq));
        clause.body = trim(text.substr(eq + 1));
        // 对 left、clause.body 的前后多余 " 或 | 做循环去除
        while (!left.empty() && (left.front() == '"' || left.front() == '|'))
        {
            left = trim(left.substr(1));
        }
        while (!left.empty() && (left.back() == '"' || left.back() == '|'))
        {
            left = trim(left.substr(0, left.size() - 1));
        }
        while (!clause.body.empty() && (clause.body.back() == '"' || clause.body.back() == '|'))
        {
            clause.body = trim(clause.body.substr(0, clause.body.size() - 1));
        }
        while (!clause.body.empty() && (clause.body.front() == '"' || clause.body.front() == '|'))
        {
            clause.body = trim(clause.body.substr(1));
        }
        std::string afterName = left;
        // 如果传入了 funName 且 left 以 funName 开头，则把 funName 从 left 前端切掉，得到仅剩参数部分
        if (!funName.empty())
        {
            if (left.rfind(funName, 0) == 0)
            {
                afterName = trim(left.substr(funName.size()));
            }
        }

        // 分割参数字符串为 tokens
        auto argStrings = splitArgumentsRespectingParens(afterName);
        for (const auto &arg : argStrings)
        {
            clause.patterns.push_back(parsePatternTerm(arg));
        }
        // 解析子句体表达式
        clause.bodyExpr = parseExpression(clause.body);
        // 返回填好 patterns、body、bodyExpr 的 FunctionClause
        return clause;
    }

    // 把一个模式文本（函数参数或 case 分支左侧的模式，例如 "_", "x", "Cons x xs", "(Suc m)"）解析为 PatternNode（Kind、name、children）
    // eg.
    // "" → Wildcard, name=""
    // "x" → Variable, name="x"
    // "Cons x xs" → Constructor name="Cons", children=[Variable("x"), Variable("xs")]
    // "(Suc (Suc n))" → Constructor Suc -> child Constructor Suc -> child Variable n
    // "None"（若 knownConstructors 包含 "None"）→ Constructor 无 children
    PatternNode parsePatternTerm(const std::string &text)
    {
        PatternNode pattern;
        std::string cleaned = trim(text);
        while (cleaned.size() >= 2 && cleaned.front() == '(' && cleaned.back() == ')' && hasBalancedParens(cleaned.substr(1, cleaned.size() - 2)))
        {
            cleaned = trim(cleaned.substr(1, cleaned.size() - 2));
        }
        // 处理空或 "_"
        if (cleaned.empty())
        {
            pattern.kind = PatternNode::Kind::Wildcard;
            pattern.name = "_";
            return pattern;
        }
        if (cleaned == "_")
        {
            pattern.kind = PatternNode::Kind::Wildcard;
            pattern.name = "_";
            return pattern;
        }
        // 按顶层空白分割 tokens
        auto tokens = splitArgumentsRespectingParens(cleaned);
        if (tokens.empty())
        {
            pattern.kind = PatternNode::Kind::Variable;
            pattern.name = cleaned;
            return pattern;
        }
        // 单 token 情况
        std::string head = tokens[0];
        // 若 tokens.size() == 1 且 token 不是已知构造子（knownConstructors 不包含该名字）
        // 把它当作 Variable：kind = Variable，name = token
        if (tokens.size() == 1 && !knownConstructors.count(head))
        {
            pattern.kind = PatternNode::Kind::Variable;
            pattern.name = head;
            return pattern;
        }
        // 若 tokens.size() == 1 且 token 是已知构造子
        // 则把它当作 Constructor（无 children）
        if (knownConstructors.count(head))
        {
            pattern.kind = PatternNode::Kind::Constructor;
            pattern.name = head;
            for (size_t i = 1; i < tokens.size(); ++i)
            {
                pattern.children.push_back(parsePatternTerm(tokens[i]));
            }
            return pattern;
        }
        // 多 token 情况
        if (tokens.size() > 1)
        {
            // 取首 token 为 head，设为 Constructor
            pattern.kind = PatternNode::Kind::Constructor;
            pattern.name = head;
            // 对后续每个 token 递归调用 parsePatternTerm，作为 children
            for (size_t i = 1; i < tokens.size(); ++i)
            {
                pattern.children.push_back(parsePatternTerm(tokens[i]));
            }
            return pattern;
        }
        pattern.kind = PatternNode::Kind::Variable;
        pattern.name = head;
        return pattern;
    }

    bool isWordBoundaryChar(char ch) const
    {
        return std::isspace(static_cast<unsigned char>(ch));
    }

    bool isKeywordBoundary(const std::string &text, size_t pos, size_t len) const
    {
        bool leftOk = (pos == 0) || isWordBoundaryChar(text[pos - 1]);
        bool rightOk = (pos + len >= text.size()) || isWordBoundaryChar(text[pos + len]);
        return leftOk && rightOk;
    }

    size_t findKeywordOutsideParens(const std::string &text, const std::string &keyword, size_t startIndex = 0) const
    {
        int depth = 0;
        for (size_t i = startIndex; i + keyword.size() <= text.size(); ++i)
        {
            char ch = text[i];
            if (ch == '(')
            {
                depth++;
            }
            else if (ch == ')')
            {
                depth--;
            }
            if (depth == 0 && text.compare(i, keyword.size(), keyword) == 0 && isKeywordBoundary(text, i, keyword.size()))
            {
                return i;
            }
        }
        return std::string::npos;
    }

    size_t findOperatorOutsideParens(const std::string &text, const std::string &op) const
    {
        int depth = 0;
        for (size_t i = 0; i + op.size() <= text.size(); ++i)
        {
            char ch = text[i];
            if (ch == '(')
            {
                depth++;
            }
            else if (ch == ')')
            {
                depth--;
            }
            if (depth == 0 && text.compare(i, op.size(), op) == 0)
            {
                return i;
            }
        }
        return std::string::npos;
    }

    Expression parseIfExpression(const std::string &text)
    {
        std::string cleaned = trim(text);
        if (cleaned.rfind("if ", 0) != 0)
        {
            return Expression::makeUnsupported(text);
        }
        std::string afterIf = trim(cleaned.substr(2));
        size_t thenPos = findKeywordOutsideParens(afterIf, "then");
        if (thenPos == std::string::npos)
        {
            return Expression::makeUnsupported(text);
        }
        size_t elsePos = findKeywordOutsideParens(afterIf, "else", thenPos + 4);
        if (elsePos == std::string::npos)
        {
            return Expression::makeUnsupported(text);
        }
        std::string conditionText = trim(afterIf.substr(0, thenPos));
        std::string thenText = trim(afterIf.substr(thenPos + 4, elsePos - (thenPos + 4)));
        std::string elseText = trim(afterIf.substr(elsePos + 4));
        Expression expr;
        expr.kind = Expression::Kind::IfThenElse;
        expr.name = "if";
        expr.arguments.push_back(parseExpression(conditionText));
        expr.arguments.push_back(parseExpression(thenText));
        expr.arguments.push_back(parseExpression(elseText));
        expr.raw = cleaned;
        return expr;
    }

    Expression parseLetExpression(const std::string &text)
    {
        std::string rest = trim(text.substr(3));
        size_t inPos = findKeywordOutsideParens(rest, "in");
        if (inPos == std::string::npos)
        {
            return Expression::makeUnsupported(text);
        }
        std::string bindingText = trim(rest.substr(0, inPos));
        std::string bodyText = trim(rest.substr(inPos + 2));
        size_t eqPos = findOperatorOutsideParens(bindingText, "=");
        if (eqPos == std::string::npos)
        {
            return Expression::makeUnsupported(text);
        }
        std::string varName = trim(bindingText.substr(0, eqPos));
        std::string valueText = trim(bindingText.substr(eqPos + 1));
        if (varName.empty())
        {
            return Expression::makeUnsupported(text);
        }
        Expression expr;
        expr.kind = Expression::Kind::LetBinding;
        expr.hasLet = true;
        expr.letVar = varName;
        expr.arguments.push_back(parseExpression(valueText));
        expr.letValueIndex = static_cast<int>(expr.arguments.size()) - 1;
        expr.arguments.push_back(parseExpression(bodyText));
        expr.letBodyIndex = static_cast<int>(expr.arguments.size()) - 1;
        expr.raw = text;
        return expr;
    }

    Expression parseCaseExpression(const std::string &text)
    {
        std::string rest = trim(text.substr(4));
        size_t ofPos = findKeywordOutsideParens(rest, "of");
        if (ofPos == std::string::npos)
        {
            return Expression::makeUnsupported(text);
        }
        std::string scrutineeText = trim(rest.substr(0, ofPos));
        std::string altsText = trim(rest.substr(ofPos + 2));
        if (!altsText.empty() && altsText.front() == '|')
        {
            altsText = trim(altsText.substr(1));
        }
        altsText = replaceAll(altsText, "\\<Rightarrow>", "=>");
        altsText = replaceAll(altsText, "⇒", "=>");
        std::vector<std::string> alternatives = splitTopLevel(altsText, '|');
        if (alternatives.empty())
        {
            return Expression::makeUnsupported(text);
        }
        Expression expr;
        expr.kind = Expression::Kind::CaseExpression;
        expr.hasCase = true;
        expr.arguments.push_back(parseExpression(scrutineeText));
        expr.caseScrutineeIndex = static_cast<int>(expr.arguments.size()) - 1;
        for (const auto &rawAlt : alternatives)
        {
            std::string altText = trim(rawAlt);
            if (altText.empty())
            {
                continue;
            }
            size_t arrowPos = findOperatorOutsideParens(altText, "=>");
            if (arrowPos == std::string::npos)
            {
                return Expression::makeUnsupported(text);
            }
            std::string patternText = trim(altText.substr(0, arrowPos));
            std::string bodyText = trim(altText.substr(arrowPos + 2));
            Expression::CaseAlternative alt;
            alt.pattern = parsePatternTerm(patternText);
            expr.arguments.push_back(parseExpression(bodyText));
            alt.bodyIndex = static_cast<int>(expr.arguments.size()) - 1;
            expr.caseAlternatives.push_back(alt);
        }
        if (expr.caseAlternatives.empty())
        {
            return Expression::makeUnsupported(text);
        }
        expr.raw = text;
        return expr;
    }

    std::optional<Expression> tryParseComparisonExpression(const std::string &text)
    {
        // 注意：较长的运算符需要先检查，否则 ">=" 会被错误地匹配为 ">"
        static const std::vector<std::string> ops = {"&&", "||", ">=", "<=", "!=", "=", "<", ">", "+", "-", "*", "/"};
        for (const auto &op : ops)
        {
            size_t pos = findOperatorOutsideParens(text, op);
            if (pos == std::string::npos)
            {
                continue;
            }
            std::string left = trim(text.substr(0, pos));
            std::string right = trim(text.substr(pos + op.size()));
            if (left.empty() || right.empty())
            {
                continue;
            }
            Expression expr;
            expr.kind = Expression::Kind::BinaryOperator;
            expr.name = op;
            expr.arguments.push_back(parseExpression(left));
            expr.arguments.push_back(parseExpression(right));
            expr.raw = text;
            return expr;
        }
        return std::nullopt;
    }

    Expression parseExpression(const std::string &text)
    {
        std::string cleaned = trim(text);
        if (cleaned.empty())
        {
            return Expression::makeUnsupported(text);
        }
        // 逻辑运算符
        cleaned = replaceAll(cleaned, "\\<and>", "&&");
        cleaned = replaceAll(cleaned, "∧", "&&");
        cleaned = replaceAll(cleaned, "\\<or>", "||");
        cleaned = replaceAll(cleaned, "∨", "||");
        cleaned = replaceAll(cleaned, "\\<Rightarrow>", "=>");
        cleaned = replaceAll(cleaned, "⇒", "=>");
        cleaned = replaceAll(cleaned, "\\<not>", "!");
        cleaned = replaceAll(cleaned, "¬", "!");
        
        // 比较运算符 - Unicode 和 Isabelle 格式
        cleaned = replaceAll(cleaned, "\\<ge>", ">=");
        cleaned = replaceAll(cleaned, "\\<le>", "<=");
        cleaned = replaceAll(cleaned, "\\<noteq>", "!=");
        cleaned = replaceAll(cleaned, "≥", ">=");
        cleaned = replaceAll(cleaned, "≤", "<=");
        cleaned = replaceAll(cleaned, "≠", "!=");
        cleaned = replaceAll(cleaned, "⊂", "<");
        cleaned = replaceAll(cleaned, "⊃", ">");
        
        // 其他常见 Unicode 数学符号
        cleaned = replaceAll(cleaned, "×", "*");
        cleaned = replaceAll(cleaned, "÷", "/");
        cleaned = replaceAll(cleaned, "−", "-");
        while (true)
        {
            auto stripped = stripOuterParens(cleaned);
            if (!stripped)
            {
                break;
            }
            cleaned = *stripped;
        }
        if (startsWithKeyword(cleaned, "if"))
        {
            Expression ifExpr = parseIfExpression(cleaned);
            if (ifExpr.kind != Expression::Kind::Unsupported)
            {
                return ifExpr;
            }
        }
        if (startsWithKeyword(cleaned, "let"))
        {
            Expression letExpr = parseLetExpression(cleaned);
            if (letExpr.kind != Expression::Kind::Unsupported)
            {
                return letExpr;
            }
        }
        if (startsWithKeyword(cleaned, "case"))
        {
            Expression caseExpr = parseCaseExpression(cleaned);
            if (caseExpr.kind != Expression::Kind::Unsupported)
            {
                return caseExpr;
            }
        }
        if (auto comparisonExpr = tryParseComparisonExpression(cleaned))
        {
            return *comparisonExpr;
        }
        auto tokens = splitArgumentsRespectingParens(cleaned);
        if (tokens.empty())
        {
            return Expression::makeUnsupported(text);
        }
        Expression expr;
        expr.name = tokens[0];
        expr.raw = cleaned;
        if (tokens.size() == 1)
        {
            if (knownConstructors.count(tokens[0]))
            {
                expr.kind = Expression::Kind::ConstructorCall;
            }
            else
            {
                expr.kind = Expression::Kind::Variable;
            }
            return expr;
        }
        for (size_t i = 1; i < tokens.size(); ++i)
        {
            expr.arguments.push_back(parseExpression(tokens[i]));
        }
        if (knownConstructors.count(expr.name))
        {
            expr.kind = Expression::Kind::ConstructorCall;
        }
        else
        {
            expr.kind = Expression::Kind::FunctionCall;
        }
        return expr;
    }
};

class CodeGenerator
{
public:
    explicit CodeGenerator(const ModuleAst &module) : ast(module)
    {
        for (const auto &datatype : ast.datatypes)
        {
            datatypeMap[datatype.name] = datatype;
            for (const auto &ctor : datatype.constructors)
            {
                constructorToDatatype[ctor.name] = datatype.name;
            }
        }
    }

    void generate(const std::string &outputDir, bool emitTestMain)
    {
        std::filesystem::create_directories(outputDir);
        const std::string singlePath = outputDir + "/" + ast.theoryName + ".cpp";
        writeSingleFile(singlePath, emitTestMain);
    }

private:
    ModuleAst ast;
    std::map<std::string, DatatypeDecl> datatypeMap;
    std::map<std::string, std::string> constructorToDatatype;
    enum class CanonicalKind
    {
        None,
        NatAdd,
        NatMul,
        ListInsertHead,
        ListInsertTail,
        ListDelete,
        ListUpdate,
        ListGet,
        ListFind,
        TreeInsert,
        TreeSearch,
        TreeMin,
        TreeDelete,
        TreeUpdate
    };
    std::vector<std::pair<CanonicalKind, std::string>> canonicalFunctions;

    void resetCanonical()
    {
        canonicalFunctions.clear();
    }

    CanonicalKind classifyCanonical(const std::string &name) const
    {
        if (name == "myadd")
        {
            return CanonicalKind::NatAdd;
        }
        if (name == "mymult")
        {
            return CanonicalKind::NatMul;
        }
        if (name == "insert_head")
        {
            return CanonicalKind::ListInsertHead;
        }
        if (name == "insert_tail")
        {
            return CanonicalKind::ListInsertTail;
        }
        if (name == "mydelete")
        {
            return CanonicalKind::ListDelete;
        }
        if (name == "myupdate")
        {
            return CanonicalKind::ListUpdate;
        }
        if (name == "myget")
        {
            return CanonicalKind::ListGet;
        }
        if (name == "myfind")
        {
            return CanonicalKind::ListFind;
        }
        if (name == "tree_insert")
        {
            return CanonicalKind::TreeInsert;
        }
        if (name == "tree_search")
        {
            return CanonicalKind::TreeSearch;
        }
        if (name == "tree_min")
        {
            return CanonicalKind::TreeMin;
        }
        if (name == "tree_delete")
        {
            return CanonicalKind::TreeDelete;
        }
        if (name == "tree_update")
        {
            return CanonicalKind::TreeUpdate;
        }
        return CanonicalKind::None;
    }

    bool hasCanonical(CanonicalKind kind) const
    {
        for (const auto &entry : canonicalFunctions)
        {
            if (entry.first == kind)
            {
                return true;
            }
        }
        return false;
    }

    void registerCanonicalFunction(const FunDecl &fun)
    {
        CanonicalKind kind = classifyCanonical(fun.name);
        if (kind == CanonicalKind::None || hasCanonical(kind))
        {
            return;
        }
        // 使用 sanitizeIdentifier 处理后的函数名
        canonicalFunctions.push_back({kind, sanitizeIdentifier(fun.name)});
    }

    struct FunctionRenderInfo
    {
        std::vector<std::string> typeVarOrder;
        std::map<std::string, std::string> typeVarMap;
        std::string returnType;
        std::vector<std::string> argumentTypes;
    };

    struct PatternBuildResult
    {
        bool supported = true;
        std::vector<std::string> conditions;
        std::vector<std::string> bindings;
    };

    void emitTemplateDecl(std::ofstream &out, size_t count) const
    {
        if (count == 0)
        {
            return;
        }
        out << "template <";
        for (size_t i = 0; i < count; ++i)
        {
            out << "typename T" << i;
            if (i + 1 < count)
            {
                out << ", ";
            }
        }
        out << ">\n";
    }

    FunctionRenderInfo buildFunctionRenderInfo(const FunDecl &fun) const
    {
        FunctionRenderInfo info;
        info.typeVarOrder = fun.typeVars;
        info.typeVarMap = buildTypeVarMap(info.typeVarOrder);
        if (!fun.returnType.name.empty())
        {
            info.returnType = renderType(fun.returnType, info.typeVarMap);
        }
        if (info.returnType.empty())
        {
            info.returnType = "void";
        }
        for (const auto &argType : fun.argumentTypes)
        {
            info.argumentTypes.push_back(renderType(argType, info.typeVarMap));
        }
        if (info.argumentTypes.empty() && !fun.clauses.empty())
        {
            size_t patternCount = fun.clauses.front().patterns.size();
            info.argumentTypes.assign(patternCount, "auto");
        }
        return info;
    }

    std::string buildArgumentList(const FunctionRenderInfo &info) const
    {
        std::string buffer;
        for (size_t i = 0; i < info.argumentTypes.size(); ++i)
        {
            if (i > 0)
            {
                buffer += ", ";
            }
            buffer += "const " + info.argumentTypes[i] + " &arg" + std::to_string(i);
        }
        return buffer;
    }

    std::string stringifyPattern(const PatternNode &node, int depth = 0) const
    {
        std::string prefix(depth * 2, ' ');
        std::string buffer = prefix;
        switch (node.kind)
        {
        case PatternNode::Kind::Wildcard:
            buffer += "_";
            break;
        case PatternNode::Kind::Variable:
            buffer += node.name;
            break;
        case PatternNode::Kind::Constructor:
            buffer += node.name;
            break;
        }
        if (!node.children.empty())
        {
            buffer += "(";
            for (size_t i = 0; i < node.children.size(); ++i)
            {
                buffer += stringifyPattern(node.children[i], 0);
                if (i + 1 < node.children.size())
                {
                    buffer += ", ";
                }
            }
            buffer += ")";
        }
        return buffer;
    }

    const DatatypeDecl *findDatatype(const std::string &name) const
    {
        auto it = datatypeMap.find(name);
        if (it == datatypeMap.end())
        {
            return nullptr;
        }
        return &it->second;
    }

    const Constructor *findConstructor(const DatatypeDecl &datatype, const std::string &name) const
    {
        for (const auto &ctor : datatype.constructors)
        {
            if (ctor.name == name)
            {
                return &ctor;
            }
        }
        return nullptr;
    }

    PatternBuildResult buildPatternCode(const PatternNode &pattern,
                                        const TypeExpr &valueType,
                                        const std::string &valueExpr,
                                        const FunctionRenderInfo &info,
                                        const std::string *tagTypeOverride = nullptr) const
    {
        PatternBuildResult result;
        if (pattern.kind == PatternNode::Kind::Wildcard)
        {
            return result;
        }
        if (pattern.kind == PatternNode::Kind::Variable)
        {
            if (!pattern.name.empty() && pattern.name != "_")
            {
                std::string typeStr = "auto";
                if (!valueType.name.empty())
                {
                    typeStr = renderType(valueType, info.typeVarMap);
                }
                // 使用 sanitizeIdentifier 处理 C++ 保留字
                std::string safeName = sanitizeIdentifier(pattern.name);
                result.bindings.push_back("const " + typeStr + " &" + safeName + " = " + valueExpr + ";");
            }
            return result;
        }
        std::string resolvedDatatypeName = valueType.name;
        const DatatypeDecl *datatype = nullptr;
        if (!resolvedDatatypeName.empty())
        {
            datatype = findDatatype(resolvedDatatypeName);
        }
        if (!datatype && pattern.kind == PatternNode::Kind::Constructor)
        {
            auto ownerIt = constructorToDatatype.find(pattern.name);
            if (ownerIt != constructorToDatatype.end())
            {
                resolvedDatatypeName = ownerIt->second;
                datatype = findDatatype(resolvedDatatypeName);
            }
        }
        if (!datatype)
        {
            result.supported = false;
            return result;
        }
        const Constructor *ctor = findConstructor(*datatype, pattern.name);
        if (!ctor)
        {
            result.supported = false;
            return result;
        }
        if (pattern.children.size() != ctor->argumentTypes.size())
        {
            result.supported = false;
            return result;
        }
        TypeExpr effectiveType = valueType;
        if (effectiveType.name.empty())
        {
            effectiveType.name = resolvedDatatypeName;
        }
        std::string typeStr = tagTypeOverride ? *tagTypeOverride : renderType(effectiveType, info.typeVarMap);
        std::string guardedValue = "(" + valueExpr + ")";
        result.conditions.push_back(guardedValue + ".tag == " + typeStr + "::Tag::" + pattern.name);
        auto substitution = buildDatatypeSubstitution(effectiveType, *datatype);
        for (size_t idx = 0; idx < pattern.children.size(); ++idx)
        {
            TypeExpr fieldType = substituteTypeVars(ctor->argumentTypes[idx], substitution);
            std::string fieldExpr = valueExpr + "." + pattern.name + "_value" + std::to_string(idx);
            if (typeExprUsesDatatype(fieldType, datatype->name))
            {
                fieldExpr = "*(" + fieldExpr + ")";
            }
            fieldExpr = "(" + fieldExpr + ")";
            PatternBuildResult childResult = buildPatternCode(pattern.children[idx], fieldType, fieldExpr, info, tagTypeOverride);
            if (!childResult.supported)
            {
                return childResult;
            }
            result.conditions.insert(result.conditions.end(), childResult.conditions.begin(), childResult.conditions.end());
            result.bindings.insert(result.bindings.end(), childResult.bindings.begin(), childResult.bindings.end());
        }
        return result;
    }

    static std::string joinStrings(const std::vector<std::string> &items, const std::string &separator)
    {
        if (items.empty())
        {
            return "";
        }
        std::ostringstream builder;
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (i > 0)
            {
                builder << separator;
            }
            builder << items[i];
        }
        return builder.str();
    }

    std::optional<std::string> buildConstructorEquality(const std::string &valueExpr,
                                                        const Expression &ctorExpr) const
    {
        auto ownerIt = constructorToDatatype.find(ctorExpr.name);
        if (ownerIt == constructorToDatatype.end())
        {
            return std::nullopt;
        }
        const std::string &datatypeName = ownerIt->second;
        const DatatypeDecl *datatype = findDatatype(datatypeName);
        if (!datatype)
        {
            return std::nullopt;
        }
        const Constructor *ctor = findConstructor(*datatype, ctorExpr.name);
        if (!ctor || !ctor->argumentTypes.empty())
        {
            return std::nullopt;
        }
        std::string wrappedValue = "(" + valueExpr + ")";
        // 使用 std::remove_reference_t 去掉引用类型，避免 decltype 返回引用类型导致编译错误
        std::string tagType = "std::remove_reference_t<decltype(" + wrappedValue + ")>";
        std::string condition = wrappedValue + ".tag == " + tagType + "::Tag::" + ctorExpr.name;
        return "(" + condition + ")";
    }

    std::optional<std::string> renderExpression(const Expression &expr,
                                                const TypeExpr *expectedType,
                                                const FunctionRenderInfo &info) const
    {
        switch (expr.kind)
        {
        case Expression::Kind::Variable:
            if (expr.name == "True")
            {
                return std::optional<std::string>("true");
            }
            if (expr.name == "False")
            {
                return std::optional<std::string>("false");
            }
            if (expr.name == "undefined")
            {
                if (!expectedType)
                {
                    return std::nullopt;
                }
                std::string typeStr = renderType(*expectedType, info.typeVarMap);
                return "isabelle_raise_undefined<" + typeStr + ">(\"undefined\")";
            }
            // 检查是否是无参数构造子（如 None, Nil 等）
            {
                auto ownerIt = constructorToDatatype.find(expr.name);
                if (ownerIt != constructorToDatatype.end() && expectedType)
                {
                    const std::string &datatypeName = ownerIt->second;
                    const DatatypeDecl *datatype = findDatatype(datatypeName);
                    if (datatype)
                    {
                        const Constructor *ctor = findConstructor(*datatype, expr.name);
                        if (ctor && ctor->argumentTypes.empty())
                        {
                            // 无参数构造子，生成 Type::make_CtorName()
                            std::string containerType = renderType(*expectedType, info.typeVarMap);
                            return containerType + "::make_" + expr.name + "()";
                        }
                    }
                }
            }
            // 使用 sanitizeIdentifier 处理 C++ 保留字
            return sanitizeIdentifier(expr.name);
        case Expression::Kind::FunctionCall:
        {
            // 首先检查是否是构造子调用（如 Some(x)）
            auto ownerIt = constructorToDatatype.find(expr.name);
            if (ownerIt != constructorToDatatype.end() && expectedType)
            {
                const std::string &datatypeName = ownerIt->second;
                const DatatypeDecl *datatype = findDatatype(datatypeName);
                if (datatype)
                {
                    const Constructor *ctor = findConstructor(*datatype, expr.name);
                    if (ctor && ctor->argumentTypes.size() == expr.arguments.size())
                    {
                        // 这是一个构造子调用，转换为 ConstructorCall 处理
                        auto substitution = buildDatatypeSubstitution(*expectedType, *datatype);
                        std::vector<std::string> renderedArgs;
                        for (size_t idx = 0; idx < expr.arguments.size(); ++idx)
                        {
                            TypeExpr fieldType = substituteTypeVars(ctor->argumentTypes[idx], substitution);
                            auto renderedChild = renderExpression(expr.arguments[idx], &fieldType, info);
                            if (!renderedChild)
                            {
                                return std::nullopt;
                            }
                            if (typeExprUsesDatatype(fieldType, datatype->name))
                            {
                                std::string innerType = renderType(fieldType, info.typeVarMap);
                                renderedArgs.push_back("std::make_shared<" + innerType + ">(" + *renderedChild + ")");
                            }
                            else
                            {
                                renderedArgs.push_back(*renderedChild);
                            }
                        }
                        std::string containerType = renderType(*expectedType, info.typeVarMap);
                        return containerType + "::make_" + expr.name + "(" + joinStrings(renderedArgs, ", ") + ")";
                    }
                }
            }
            
            // 普通函数调用
            std::vector<std::string> renderedArgs;
            for (const auto &arg : expr.arguments)
            {
                auto rendered = renderExpression(arg, nullptr, info);
                if (!rendered)
                {
                    return std::nullopt;
                }
                renderedArgs.push_back(*rendered);
            }
            // 使用 sanitizeIdentifier 处理函数名中可能的保留字和 std 冲突
            std::string safeFuncName = sanitizeIdentifier(expr.name);
            std::string call = safeFuncName + "(" + joinStrings(renderedArgs, ", ") + ")";
            return call;
        }
        case Expression::Kind::BinaryOperator:
        {
            if (expr.arguments.size() != 2)
            {
                return std::nullopt;
            }
            if (expr.name == "=")
            {
                const Expression &lhsExpr = expr.arguments[0];
                const Expression &rhsExpr = expr.arguments[1];
                if (rhsExpr.kind == Expression::Kind::ConstructorCall)
                {
                    auto lhsValue = renderExpression(lhsExpr, nullptr, info);
                    if (!lhsValue)
                    {
                        return std::nullopt;
                    }
                    if (auto equality = buildConstructorEquality(*lhsValue, rhsExpr))
                    {
                        return *equality;
                    }
                }
                if (lhsExpr.kind == Expression::Kind::ConstructorCall)
                {
                    auto rhsValue = renderExpression(rhsExpr, nullptr, info);
                    if (!rhsValue)
                    {
                        return std::nullopt;
                    }
                    if (auto equality = buildConstructorEquality(*rhsValue, lhsExpr))
                    {
                        return *equality;
                    }
                }
            }
            auto lhs = renderExpression(expr.arguments[0], nullptr, info);
            auto rhs = renderExpression(expr.arguments[1], nullptr, info);
            if (!lhs || !rhs)
            {
                return std::nullopt;
            }
            std::string op = expr.name;
            if (op == "=")
            {
                op = "==";
            }
            return "(" + *lhs + " " + op + " " + *rhs + ")";
        }
        case Expression::Kind::IfThenElse:
        {
            if (expr.arguments.size() != 3)
            {
                return std::nullopt;
            }
            auto condition = renderExpression(expr.arguments[0], nullptr, info);
            auto thenExpr = renderExpression(expr.arguments[1], expectedType, info);
            auto elseExpr = renderExpression(expr.arguments[2], expectedType, info);
            if (!condition || !thenExpr || !elseExpr)
            {
                return std::nullopt;
            }
            return "((" + *condition + ") ? (" + *thenExpr + ") : (" + *elseExpr + "))";
        }
        case Expression::Kind::LetBinding:
        {
            if (!expr.hasLet || expr.letValueIndex < 0 || expr.letBodyIndex < 0)
            {
                return std::nullopt;
            }
            if (expr.letValueIndex >= static_cast<int>(expr.arguments.size()) || expr.letBodyIndex >= static_cast<int>(expr.arguments.size()))
            {
                return std::nullopt;
            }
            auto valueExpr = renderExpression(expr.arguments[expr.letValueIndex], nullptr, info);
            auto bodyExpr = renderExpression(expr.arguments[expr.letBodyIndex], expectedType, info);
            if (!valueExpr || !bodyExpr)
            {
                return std::nullopt;
            }
            std::ostringstream builder;
            builder << "([&]{ auto " << expr.letVar << " = " << *valueExpr << "; return " << *bodyExpr << "; }())";
            return builder.str();
        }
        case Expression::Kind::CaseExpression:
        {
            if (!expr.hasCase || expr.caseScrutineeIndex < 0 || expr.caseScrutineeIndex >= static_cast<int>(expr.arguments.size()))
            {
                return std::nullopt;
            }
            auto scrutineeExpr = renderExpression(expr.arguments[expr.caseScrutineeIndex], nullptr, info);
            if (!scrutineeExpr)
            {
                return std::nullopt;
            }
            static int caseCounter = 0;
            std::string tempVar = "caseTemp" + std::to_string(caseCounter++);
            std::ostringstream builder;
            builder << "([&]{ auto " << tempVar << " = " << *scrutineeExpr << "; ";
            bool emitted = false;
            for (const auto &alt : expr.caseAlternatives)
            {
                TypeExpr inferredType;
                if (alt.pattern.kind == PatternNode::Kind::Constructor)
                {
                    auto ownerIt = constructorToDatatype.find(alt.pattern.name);
                    if (ownerIt != constructorToDatatype.end())
                    {
                        inferredType.name = ownerIt->second;
                    }
                }
                std::string overrideType = "std::remove_reference_t<decltype(" + tempVar + ")>";
                PatternBuildResult patternResult = buildPatternCode(alt.pattern, inferredType, tempVar, info, &overrideType);
                if (!patternResult.supported)
                {
                    return std::nullopt;
                }
                std::vector<std::string> wrappedConditions;
                for (const auto &condition : patternResult.conditions)
                {
                    wrappedConditions.push_back("(" + condition + ")");
                }
                if (!wrappedConditions.empty())
                {
                    builder << "if (" << joinStrings(wrappedConditions, " && ") << ") {\n";
                }
                else
                {
                    builder << "if (true) {\n";
                }
                for (const auto &binding : patternResult.bindings)
                {
                    builder << "    " << binding << "\n";
                }
                if (alt.bodyIndex < 0 || alt.bodyIndex >= static_cast<int>(expr.arguments.size()))
                {
                    return std::nullopt;
                }
                auto bodyExpr = renderExpression(expr.arguments[alt.bodyIndex], expectedType, info);
                if (!bodyExpr)
                {
                    return std::nullopt;
                }
                builder << "    return " << *bodyExpr << ";\n";
                builder << "}\n";
                emitted = true;
            }
            builder << "    throw std::runtime_error(\"case expression did not match\");\n";
            builder << "}())";
            return builder.str();
        }
        case Expression::Kind::ConstructorCall:
        {
            if (!expectedType)
            {
                return std::nullopt;
            }
            auto ownerIt = constructorToDatatype.find(expr.name);
            if (ownerIt == constructorToDatatype.end())
            {
                return std::nullopt;
            }
            const std::string &datatypeName = ownerIt->second;
            if (expectedType->name != datatypeName)
            {
                return std::nullopt;
            }
            const DatatypeDecl *datatype = findDatatype(datatypeName);
            if (!datatype)
            {
                return std::nullopt;
            }
            const Constructor *ctor = findConstructor(*datatype, expr.name);
            if (!ctor)
            {
                return std::nullopt;
            }
            if (expr.arguments.size() != ctor->argumentTypes.size())
            {
                return std::nullopt;
            }
            auto substitution = buildDatatypeSubstitution(*expectedType, *datatype);
            std::vector<std::string> renderedArgs;
            for (size_t idx = 0; idx < expr.arguments.size(); ++idx)
            {
                TypeExpr fieldType = substituteTypeVars(ctor->argumentTypes[idx], substitution);
                auto renderedChild = renderExpression(expr.arguments[idx], &fieldType, info);
                if (!renderedChild)
                {
                    return std::nullopt;
                }
                std::string storageType = renderDatatypeFieldType(fieldType, info.typeVarMap, datatype->name);
                if (typeExprUsesDatatype(fieldType, datatype->name))
                {
                    std::string innerType = renderType(fieldType, info.typeVarMap);
                    renderedArgs.push_back("std::make_shared<" + innerType + ">(" + *renderedChild + ")");
                }
                else
                {
                    renderedArgs.push_back(*renderedChild);
                }
            }
            std::string containerType = renderType(*expectedType, info.typeVarMap);
            std::string result = containerType + "::make_" + expr.name + "(" + joinStrings(renderedArgs, ", ") + ")";
            return result;
        }
        default:
            return std::nullopt;
        }
    }

    std::optional<std::string> buildClauseBlock(const FunDecl &fun,
                                                const FunctionClause &clause,
                                                const FunctionRenderInfo &info) const
    {
        if (clause.patterns.size() > fun.argumentTypes.size())
        {
            return std::nullopt;
        }
        std::vector<std::string> conditions;
        std::vector<std::string> bindings;
        for (size_t idx = 0; idx < clause.patterns.size(); ++idx)
        {
            const auto &pattern = clause.patterns[idx];
            const TypeExpr &typeExpr = idx < fun.argumentTypes.size() ? fun.argumentTypes[idx] : TypeExpr();
            std::string valueExpr = "arg" + std::to_string(idx);
            PatternBuildResult patternResult = buildPatternCode(pattern, typeExpr, valueExpr, info);
            if (!patternResult.supported)
            {
                return std::nullopt;
            }
            conditions.insert(conditions.end(), patternResult.conditions.begin(), patternResult.conditions.end());
            bindings.insert(bindings.end(), patternResult.bindings.begin(), patternResult.bindings.end());
        }
        TypeExpr returnTypeExpr = fun.returnType;
        auto renderedBody = renderExpression(clause.bodyExpr, &returnTypeExpr, info);
        if (!renderedBody)
        {
            return std::nullopt;
        }
        std::vector<std::string> wrappedConditions;
        for (const auto &condition : conditions)
        {
            wrappedConditions.push_back("(" + condition + ")");
        }
        std::ostringstream block;
        if (!wrappedConditions.empty())
        {
            block << "    if (" << joinStrings(wrappedConditions, " && ") << ") {\n";
        }
        else
        {
            block << "    {\n";
        }
        for (const auto &binding : bindings)
        {
            block << "        " << binding << "\n";
        }
        block << "        return " << *renderedBody << ";\n";
        block << "    }\n";
        return block.str();
    }

    // 检查类型表达式是否包含 option 类型
    bool typeExprContainsOption(const TypeExpr &type) const
    {
        if (type.name == "option")
        {
            return true;
        }
        for (const auto &arg : type.arguments)
        {
            if (typeExprContainsOption(arg))
            {
                return true;
            }
        }
        return false;
    }

    // 检测是否需要生成内置 option 类型
    bool checkNeedsOptionType(const ModuleAst &ast) const
    {
        // 首先检查是否已经有用户自定义的 option 类型
        for (const auto &dt : ast.datatypes)
        {
            if (dt.name == "option")
            {
                return false; // 用户已定义 option，无需生成内置类型
            }
        }
        // 检查函数返回类型和参数类型是否使用了 option
        for (const auto &fun : ast.functions)
        {
            if (typeExprContainsOption(fun.returnType))
            {
                return true;
            }
            for (const auto &argType : fun.argumentTypes)
            {
                if (typeExprContainsOption(argType))
                {
                    return true;
                }
            }
        }
        return false;
    }

    // 生成内置 option 类型
    void emitBuiltinOptionType(std::ofstream &out)
    {
        out << "// Built-in Isabelle option type\n";
        out << "template <typename T0>\n";
        out << "struct option {\n";
        out << "    enum class Tag {\n";
        out << "        None,\n";
        out << "        Some\n";
        out << "    };\n";
        out << "    Tag tag;\n";
        out << "    T0 Some_value0;\n";
        out << "    bool is_None() const { return tag == Tag::None; }\n";
        out << "    bool is_Some() const { return tag == Tag::Some; }\n";
        out << "\n";
        out << "    static option make_None() {\n";
        out << "        option value;\n";
        out << "        value.tag = Tag::None;\n";
        out << "        return value;\n";
        out << "    }\n";
        out << "    static option make_Some(const T0 &value0) {\n";
        out << "        option value;\n";
        out << "        value.tag = Tag::Some;\n";
        out << "        value.Some_value0 = value0;\n";
        out << "        return value;\n";
        out << "    }\n";
        out << "\n";
        out << "    // Equality operator\n";
        out << "    friend bool operator==(const option &lhs, const option &rhs) {\n";
        out << "        if (lhs.tag != rhs.tag) return false;\n";
        out << "        if (lhs.tag == Tag::Some) {\n";
        out << "            if (!(lhs.Some_value0 == rhs.Some_value0)) return false;\n";
        out << "        }\n";
        out << "        return true;\n";
        out << "    }\n";
        out << "    friend bool operator!=(const option &lhs, const option &rhs) {\n";
        out << "        return !(lhs == rhs);\n";
        out << "    }\n";
        out << "};\n\n";
        out << "template <typename T0>\n";
        out << "inline option<T0> make_option_None() {\n";
        out << "    return option<T0>::make_None();\n";
        out << "}\n\n";
        out << "template <typename T0>\n";
        out << "inline option<T0> make_option_Some(const T0 &value0) {\n";
        out << "    return option<T0>::make_Some(value0);\n";
        out << "}\n\n";
        
        // 注册 option 类型到 constructorToDatatype 映射
        constructorToDatatype["None"] = "option";
        constructorToDatatype["Some"] = "option";
        
        // 注册 option 类型到 datatypeMap
        DatatypeDecl optionDecl;
        optionDecl.name = "option";
        optionDecl.typeParams.push_back("'a");
        
        Constructor noneCttor;
        noneCttor.name = "None";
        optionDecl.constructors.push_back(noneCttor);
        
        Constructor someCttor;
        someCttor.name = "Some";
        TypeExpr someArg;
        someArg.name = "a";
        someArg.isTypeVar = true;
        someCttor.argumentTypes.push_back(someArg);
        optionDecl.constructors.push_back(someCttor);
        
        datatypeMap["option"] = optionDecl;
    }

    void writeSingleFile(const std::string &path, bool emitTestMain)
    {
        std::ofstream out(path);
        if (!out.is_open())
        {
            throw std::runtime_error("failed to open file for writing: " + path);
        }
        resetCanonical();
        out << "#include <iostream>\n";
        out << "#include <memory>\n";
        out << "#include <stdexcept>\n";
        out << "#include <string>\n";
        out << "#include <type_traits>\n";
        out << "#include <vector>\n\n";
        out << "using namespace std;\n\n";
        out << "// auto-generated from theory " << ast.theoryName << "\n\n";
        out << "template <typename T>\n";
        out << "inline T isabelle_raise_undefined(const char *context)\n";
        out << "{\n";
        out << "    throw std::runtime_error(context ? context : \"Isabelle undefined value\");\n";
        out << "}\n\n";
        
        // 检测是否需要生成内置 option 类型
        bool needsOptionType = checkNeedsOptionType(ast);
        if (needsOptionType)
        {
            emitBuiltinOptionType(out);
        }
        
        for (const auto &datatype : ast.datatypes)
        {
            emitDatatypeHeader(out, datatype);
        }
        out << "// function declarations\n";
        for (const auto &fun : ast.functions)
        {
            emitFunctionSignature(out, fun);
        }
        out << "// function implementations converted from Isabelle/HOL\n\n";
        for (const auto &fun : ast.functions)
        {
            emitFunctionBody(out, fun);
        }
        emitCanonicalWrappers(out);
        if (emitTestMain)
        {
            emitTestHarness(out);
        }
    }

    void emitDatatypeHeader(std::ofstream &out, const DatatypeDecl &decl)
    {
        out << "// datatype " << decl.name << "\n";
        auto normalized = normalizeTypeParams(decl.typeParams);
        if (!normalized.empty())
        {
            emitTemplateDecl(out, normalized.size());
        }
        auto typeVarMap = buildTypeVarMap(normalized);
        out << "struct " << decl.name << " {\n";
        out << "    enum class Tag {\n";
        for (size_t i = 0; i < decl.constructors.size(); ++i)
        {
            out << "        " << decl.constructors[i].name;
            if (i + 1 < decl.constructors.size())
            {
                out << ",\n";
            }
            else
            {
                out << "\n";
            }
        }
        out << "    };\n";
        out << "    Tag tag;\n";
        for (const auto &ctor : decl.constructors)
        {
            for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
            {
                std::string fieldType = renderDatatypeFieldType(ctor.argumentTypes[idx], typeVarMap, decl.name);
                out << "    " << fieldType << " " << ctor.name << "_value" << idx << ";\n";
            }
        }
        for (const auto &ctor : decl.constructors)
        {
            out << "    bool is_" << ctor.name << "() const { return tag == Tag::" << ctor.name << "; }\n";
        }
        out << "\n";
        for (const auto &ctor : decl.constructors)
        {
            out << "    static " << decl.name << " make_" << ctor.name << "(";
            out << buildConstructorParamList(ctor, typeVarMap, decl.name);
            out << ") {\n";
            out << "        " << decl.name << " value;\n";
            out << "        value.tag = Tag::" << ctor.name << ";\n";
            for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
            {
                out << "        value." << ctor.name << "_value" << idx << " = value" << idx << ";\n";
            }
            out << "        return value;\n";
            out << "    }\n";
        }
        // 生成 operator== 用于比较两个相同类型的值
        out << "\n    // Equality operator\n";
        out << "    friend bool operator==(const " << decl.name << " &lhs, const " << decl.name << " &rhs) {\n";
        out << "        if (lhs.tag != rhs.tag) return false;\n";
        for (const auto &ctor : decl.constructors)
        {
            if (!ctor.argumentTypes.empty())
            {
                out << "        if (lhs.tag == Tag::" << ctor.name << ") {\n";
                for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
                {
                    std::string fieldName = ctor.name + "_value" + std::to_string(idx);
                    bool isRecursive = typeExprUsesDatatype(ctor.argumentTypes[idx], decl.name);
                    if (isRecursive)
                    {
                        // 递归字段需要解引用 shared_ptr 进行比较
                        out << "            if (!(*lhs." << fieldName << " == *rhs." << fieldName << ")) return false;\n";
                    }
                    else
                    {
                        out << "            if (!(lhs." << fieldName << " == rhs." << fieldName << ")) return false;\n";
                    }
                }
                out << "            return true;\n";
                out << "        }\n";
            }
        }
        out << "        return true;\n";
        out << "    }\n";
        out << "    friend bool operator!=(const " << decl.name << " &lhs, const " << decl.name << " &rhs) {\n";
        out << "        return !(lhs == rhs);\n";
        out << "    }\n";
        out << "};\n\n";
        emitDatatypeHelpers(out, decl, normalized, typeVarMap);
    }

    std::string buildConstructorParamList(const Constructor &ctor,
                                          const std::map<std::string, std::string> &typeVarMap,
                                          const std::string &datatypeName) const
    {
        std::string buffer;
        for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
        {
            if (idx > 0)
            {
                buffer += ", ";
            }
            std::string fieldType = renderDatatypeFieldType(ctor.argumentTypes[idx], typeVarMap, datatypeName);
            buffer += "const " + fieldType + " &value" + std::to_string(idx);
        }
        return buffer;
    }

    std::string renderDatatypeInstance(const DatatypeDecl &decl, size_t paramCount) const
    {
        if (paramCount == 0)
        {
            return decl.name;
        }
        std::ostringstream oss;
        oss << decl.name << "<";
        for (size_t i = 0; i < paramCount; ++i)
        {
            if (i > 0)
            {
                oss << ", ";
            }
            oss << "T" << i;
        }
        oss << ">";
        return oss.str();
    }

    std::string buildHelperParamList(const Constructor &ctor,
                                     const std::map<std::string, std::string> &typeVarMap) const
    {
        std::vector<std::string> parts;
        for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
        {
            std::string fieldType = renderType(ctor.argumentTypes[idx], typeVarMap);
            parts.push_back("const " + fieldType + " &value" + std::to_string(idx));
        }
        return joinStrings(parts, ", ");
    }

    std::string buildHelperCtorArgs(const Constructor &ctor,
                                    const std::map<std::string, std::string> &typeVarMap,
                                    const DatatypeDecl &decl) const
    {
        std::vector<std::string> parts;
        for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
        {
            const auto &typeExpr = ctor.argumentTypes[idx];
            std::string paramName = "value" + std::to_string(idx);
            if (typeExprUsesDatatype(typeExpr, decl.name))
            {
                std::string innerType = renderType(typeExpr, typeVarMap);
                parts.push_back("std::make_shared<" + innerType + ">(" + paramName + ")");
            }
            else
            {
                parts.push_back(paramName);
            }
        }
        return joinStrings(parts, ", ");
    }

    void emitDatatypeHelpers(std::ofstream &out,
                             const DatatypeDecl &decl,
                             const std::vector<std::string> &normalized,
                             const std::map<std::string, std::string> &typeVarMap)
    {
        auto typeName = renderDatatypeInstance(decl, normalized.size());
        auto emitTemplateIfNeeded = [&]()
        {
            if (!normalized.empty())
            {
                emitTemplateDecl(out, normalized.size());
            }
        };

        for (const auto &ctor : decl.constructors)
        {
            emitTemplateIfNeeded();
            out << "inline " << typeName << " make_" << decl.name << "_" << ctor.name << "("
                << buildHelperParamList(ctor, typeVarMap) << ") {\n";
            out << "    return " << typeName << "::make_" << ctor.name << "("
                << buildHelperCtorArgs(ctor, typeVarMap, decl) << ");\n";
            out << "}\n\n";
        }

        for (const auto &ctor : decl.constructors)
        {
            emitTemplateIfNeeded();
            out << "inline bool is_" << decl.name << "_" << ctor.name << "(const " << typeName << " &value) {\n";
            out << "    return value.tag == " << typeName << "::Tag::" << ctor.name << ";\n";
            out << "}\n\n";
        }

        for (const auto &ctor : decl.constructors)
        {
            for (size_t idx = 0; idx < ctor.argumentTypes.size(); ++idx)
            {
                emitTemplateIfNeeded();
                std::string fieldType = renderType(ctor.argumentTypes[idx], typeVarMap);
                out << "inline const " << fieldType << " &get_" << decl.name << "_" << ctor.name << "_value" << idx
                    << "(const " << typeName << " &value) {\n";
                if (typeExprUsesDatatype(ctor.argumentTypes[idx], decl.name))
                {
                    out << "    return *(value." << ctor.name << "_value" << idx << ");\n";
                }
                else
                {
                    out << "    return value." << ctor.name << "_value" << idx << ";\n";
                }
                out << "}\n\n";
            }
        }
    }

    void emitFunctionSignature(std::ofstream &out, const FunDecl &fun)
    {
        auto info = buildFunctionRenderInfo(fun);
        registerCanonicalFunction(fun);
        std::string safeName = sanitizeIdentifier(fun.name);
        out << "// " << fun.name << " :: " << fun.signature << "\n";
        if (!info.typeVarOrder.empty())
        {
            emitTemplateDecl(out, info.typeVarOrder.size());
        }
        out << info.returnType << " " << safeName << "(" << buildArgumentList(info) << ");\n\n";
    }

    bool tryEmitFunctionImplementation(std::ofstream &out, const FunDecl &fun, const FunctionRenderInfo &info)
    {
        std::vector<std::string> clauseBlocks;
        for (const auto &clause : fun.clauses)
        {
            auto block = buildClauseBlock(fun, clause, info);
            if (!block)
            {
                return false;
            }
            clauseBlocks.push_back(*block);
        }
        if (clauseBlocks.empty())
        {
            return false;
        }
        std::string safeName = sanitizeIdentifier(fun.name);
        if (!info.typeVarOrder.empty())
        {
            emitTemplateDecl(out, info.typeVarOrder.size());
        }
        out << info.returnType << " " << safeName << "(" << buildArgumentList(info) << ") {\n";
        for (const auto &block : clauseBlocks)
        {
            out << block;
        }
        out << "    throw std::runtime_error(\"pattern match failure in " << fun.name << "\");\n";
        out << "}\n\n";
        return true;
    }

    void emitFunctionBody(std::ofstream &out, const FunDecl &fun)
    {
        auto info = buildFunctionRenderInfo(fun);
        if (tryEmitFunctionImplementation(out, fun, info))
        {
            return;
        }
        std::string safeName = sanitizeIdentifier(fun.name);
        if (!info.typeVarOrder.empty())
        {
            emitTemplateDecl(out, info.typeVarOrder.size());
        }
        out << info.returnType << " " << safeName << "(" << buildArgumentList(info) << ") {\n";
        out << "    // patterns translated from Isabelle/HOL\n";
        for (const auto &clause : fun.clauses)
        {
            out << "    // clause:";
            for (const auto &pattern : clause.patterns)
            {
                out << " " << stringifyPattern(pattern);
            }
            out << " => " << clause.body << "\n";
        }
        out << "    throw std::runtime_error(\"function " << fun.name << " not implemented yet\");\n";
        out << "}\n\n";
    }

    void emitCanonicalWrappers(std::ofstream &out)
    {
        for (const auto &entry : canonicalFunctions)
        {
            switch (entry.first)
            {
            case CanonicalKind::NatAdd:
                out << "mynat wrapper_add(const mynat &a, const mynat &b) { return " << entry.second << "(a, b); }\n\n";
                break;
            case CanonicalKind::NatMul:
                out << "mynat wrapper_mul(const mynat &a, const mynat &b) { return " << entry.second << "(a, b); }\n\n";
                break;
            case CanonicalKind::TreeInsert:
                out << "mytree<int> wrapper_tree_insert(int value, const mytree<int> &tree) { return " << entry.second << "(value, tree); }\n\n";
                break;
            case CanonicalKind::TreeSearch:
                out << "myoption<int> wrapper_tree_search(int value, const mytree<int> &tree) { return " << entry.second << "(value, tree); }\n\n";
                break;
            case CanonicalKind::TreeMin:
                out << "int wrapper_tree_min(const mytree<int> &tree) { return " << entry.second << "(tree); }\n\n";
                break;
            case CanonicalKind::TreeDelete:
                out << "mytree<int> wrapper_tree_delete(int value, const mytree<int> &tree) { return " << entry.second << "(value, tree); }\n\n";
                break;
            case CanonicalKind::TreeUpdate:
                out << "myoption<mytree<int>> wrapper_tree_update(int oldValue, int newValue, const mytree<int> &tree) { return " << entry.second << "(oldValue, newValue, tree); }\n\n";
                break;
            case CanonicalKind::ListInsertHead:
                out << "template <typename T> mylist<T> wrapper_list_insert_head(const T &value, const mylist<T> &lst) { return insert_head(value, lst); }\n\n";
                break;
            case CanonicalKind::ListInsertTail:
                out << "template <typename T> mylist<T> wrapper_list_insert_tail(const T &value, const mylist<T> &lst) { return insert_tail(value, lst); }\n\n";
                break;
            case CanonicalKind::ListDelete:
                out << "template <typename T> mylist<T> wrapper_list_delete(const T &value, const mylist<T> &lst) { return mydelete(value, lst); }\n\n";
                break;
            case CanonicalKind::ListUpdate:
                out << "template <typename T> mylist<T> wrapper_list_update(const mynat &index, const T &value, const mylist<T> &lst) { return myupdate(index, value, lst); }\n\n";
                break;
            case CanonicalKind::ListGet:
                out << "template <typename T> myoption<T> wrapper_list_get(const mynat &index, const mylist<T> &lst) { return myget(index, lst); }\n\n";
                break;
            case CanonicalKind::ListFind:
                out << "template <typename T> bool wrapper_list_find(const T &value, const mylist<T> &lst) { return myfind(value, lst); }\n\n";
                break;
            default:
                break;
            }
        }
    }

    void emitTestHarness(std::ofstream &out)
    {
        struct TestStub
        {
            CanonicalKind kind;
            const char *name;
            const char *note;
        };
        static const TestStub stubs[] = {
            {CanonicalKind::NatAdd, "test_add", "使用 wrapper_add 做加法测试"},
            {CanonicalKind::NatMul, "test_mul", "使用 wrapper_mul 做乘法测试"},
            {CanonicalKind::ListInsertHead, "test_list_insert_head", "测试列表头插入"},
            {CanonicalKind::ListInsertTail, "test_list_insert_tail", "测试列表尾插入"},
            {CanonicalKind::ListDelete, "test_list_delete", "测试列表删除"},
            {CanonicalKind::ListUpdate, "test_list_update", "测试列表更新"},
            {CanonicalKind::ListGet, "test_list_get", "测试列表取值"},
            {CanonicalKind::ListFind, "test_list_find", "测试列表查找"},
            {CanonicalKind::TreeInsert, "test_tree_insert", "测试树插入"},
            {CanonicalKind::TreeSearch, "test_tree_search", "测试树查找"},
            {CanonicalKind::TreeMin, "test_tree_min", "测试树最小值"},
            {CanonicalKind::TreeDelete, "test_tree_delete", "测试树删除"},
            {CanonicalKind::TreeUpdate, "test_tree_update", "测试树更新"}};

        std::vector<std::string> testFunctions;
        for (const auto &stub : stubs)
        {
            if (!hasCanonical(stub.kind))
            {
                continue;
            }
            out << "void " << stub.name << "() {\n";
            out << "    // TODO: " << stub.note << "\n";
            out << "}\n\n";
            testFunctions.push_back(stub.name);
        }

        out << "int main() {\n";
        if (testFunctions.empty())
        {
            out << "    std::cout << \"没有可用的测试函数\" << std::endl;\n";
        }
        else
        {
            out << "    std::cout << \"开始运行示例测试函数\" << std::endl;\n";
            for (const auto &fn : testFunctions)
            {
                out << "    " << fn << "();\n";
            }
            out << "    std::cout << \"测试函数执行结束\" << std::endl;\n";
        }
        out << "    return 0;\n";
        out << "}\n\n";
    }
};

struct CliOptions
{
    std::string inputPath;
    std::string outputDir = "generated";
    bool emitTestMain = false;
};

static CliOptions resolveCliOptions(int argc, char **argv)
{
    CliOptions options;
    if (argc >= 2)
    {
        options.inputPath = argv[1];
    }
    while (options.inputPath.empty())
    {
        options.inputPath = promptLine("输入 .thy 路径：");
        if (options.inputPath.empty())
        {
            std::cout << "路径不能为空\n";
        }
    }
    if (argc >= 3)
    {
        options.outputDir = argv[2];
    }
    else
    {
        std::string dir = promptLine("输出目录(默认 generated)：");
        if (!dir.empty())
        {
            options.outputDir = dir;
        }
    }
    if (argc >= 4)
    {
        std::string flag = argv[3];
        if (flag == "--test" || flag == "-t")
        {
            options.emitTestMain = true;
        }
    }
    return options;
}

int main(int argc, char **argv)
{
    try
    {
        setupConsole();
        CliOptions options = resolveCliOptions(argc, argv);
        Parser parser(options.inputPath);
        ModuleAst ast = parser.parse();
        std::string finalDir = options.outputDir + "/" + ast.theoryName;
        CodeGenerator generator(ast);
        generator.generate(finalDir, options.emitTestMain);
        std::cout << "C++ 代码已生成，位置： " << finalDir << "\n";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "converter error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
