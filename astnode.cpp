#include "astnode.hpp"

ast_node_info::ast_node_info()
{
    this->identifier = "";
}

ast_node_info::ast_node_info(std::string str)
{
    this->identifier = str;
    this->literal = str;
}

ast_node_info::ast_node_info(std::string str, identifier_type type)
{
    this->identifier = str;
    this->type = type;
}

ast_node_info::ast_node_info(int number)
{
    this->number = number;
}

ast_node::ast_node()
{
    this->info = ast_node_info("");
    this->type = AST_BLOCK;
}

ast_node::ast_node(ast_type type)
{
    this->info = ast_node_info("");
    this->type = type;
}

ast_node::ast_node(ast_type type, ast_node_info info)
{
    this->type = type;
    this->info = info;
}

void ast_node::add_child(ast_node child)
{
    this->children.push_back(child);
}

void ast_node::add_children(std::vector<ast_node> children)
{
    for (std::size_t i = 0; i < children.size(); i++)
    {
        add_child(children[i]);
    }
}

void ast_node::print(int level)
{
    for (int i = 0; i < level; i++)
        std::cout << "  ";
    std::cout << type << " ";

    if (info.identifier != "")
    {
        std::cout << info.identifier;
    }

    if (type == AST_NUMBER)
    {
        std::cout << info.number;
    }

    if (children.size())
    {
        std::cout << " my children:\n";
        ++level;
        for (std::size_t i = 0; i < children.size(); i++)
        {
            children[i].print(level);
        }
    }
    else
    {
        std::cout << "\n";
    }
}

void ast_node::print()
{
    print(0);
}