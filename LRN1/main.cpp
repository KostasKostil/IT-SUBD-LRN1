#include <bits/stdc++.h>

#define fi first
#define se second
#define pb push_back

using namespace std;

void help(ostream& outstream)
{
    outstream<<"while in database:\n -create table_name;\n -view;\n -choose table_name;\n -rename previous_table_name new_table_name;\n -union table_name1 table_name2;\n -delete table_name;\n";
    outstream<<"while in table:\n -view;\n -addcol name type;\n -addrow row_description;\n -exit;\n";
}

enum Type
{
    Integer,
    Real,
    Char,
    String,
    IntegerInterval,
    TextFile
};
vector<pair<Type, string> > types = {
{Type::Integer, "int"},
{Type::Real, "real"},
{Type::Char, "char"},
{Type::String, "string"},
{Type::IntegerInterval, "interval"},
{Type::TextFile, "text"},
};

void viewtype(Type t, ostream& outstream)
{
    for (auto pa : types)
        if (pa.fi == t)
            outstream<<pa.se;
}

struct Data
{
    int integer_value;
    double real_value;
    char char_value;
    string string_value;
    pair<int, int> interval_value;
    pair<string, string> text_value;
    ~Data() {}
};

struct Value
{
    Type type;
    Data data;

    void init()
    {
        if (type == Type::Integer) data.integer_value = 0;
        if (type == Type::Real) data.real_value = 0;
        if (type == Type::Char) data.char_value = '0';
        if (type == Type::String) data.string_value = "";
        if (type == Type::IntegerInterval) data.interval_value = {0, 0};
        if (type == Type::TextFile) data.text_value = {"", ""};
    }
    void view(ostream& outstream)
    {
        outstream<<"{ ";
        for (auto pa : types)
            if (pa.fi == type)
                outstream<<pa.se;
        outstream<<", ";
        if (type == Type::Integer) outstream<<data.integer_value;
        if (type == Type::Real) outstream<<data.real_value;
        if (type == Type::Char) outstream<<data.char_value;
        if (type == Type::String) outstream<<data.string_value;
        if (type == Type::IntegerInterval) outstream<<data.interval_value.first<<"-"<<data.interval_value.second;
        if (type == Type::TextFile) outstream<<data.text_value.first<<"->"<<data.text_value.second;
        outstream<<" }";
    }
};

struct Header
{
    Type type;
    string name;
    void view(ostream& outstream)
    {
        outstream<<"{ "<<name<<", ";
        viewtype(type, outstream);
        outstream<<" }";
    }
};

void viewrow(vector<Value> rows, ostream& outstream)
{
    cout<<"[ ";
    for (Value& v : rows)
        v.view(outstream);
    cout<<" ]";
}

struct Table
{
    string name;
    vector<Header> header;
    vector<vector<Value> > rows;

    void view_command(ostream& outstream)
    {
        outstream<<"table consists of "<<rows.size()<<" rows. They are listed below.\n";
        outstream<<"header: [";
        for (auto& h : header)
            h.view(outstream);
        outstream<<"] \n";
        for (auto& r : rows)
            viewrow(r, outstream);
    }
    void addcol_command(istream& instream, ostream& outstream)
    {
        string name;
        instream>>name;

        string tp;
        instream>>tp;

        Type t;
        for (auto& pa : types)
            if (pa.se == tp)
                t = pa.fi;

        header.push_back(Header{t, name});
        for (auto& r : rows)
        {
            Value v;
            v.type = t;
            v.init();

            r.push_back(v);
        }
    }
    void addrow_command(istream& instream, ostream& outstream)
    {

    }
    bool process_command(istream& instream, ostream& outstream)
    { // 1 if exit
        string command;
        instream>>command;

        if (command == "help")
            help(outstream);
        else if (command == "exit")
            return true;
        else if (command == "view")
            view_command(outstream);
        else if (command == "addcol")
            addcol_command(instream, outstream);
        else if (command == "addrow")
            addrow_command(instream, outstream);
        else
            outstream<<"error: incorrect command: "<<command<<"\n";

        return false;
    }
};

struct Database
{
    unordered_map<string, Table> tables;
    string chosen_table;
    bool is_table_chosen = false;

    void create_command(istream& instream, ostream& outstream)
    {
        string name;
        instream>>name;
        if (tables.count(name))
        {
            outstream<<"error: table "<<name<<" already exists\n";
            return;
        }
        tables[name];
    }
    void view_command(ostream& outstream)
    {
        outstream<<"database consists of "<<tables.size()<<" tables. Their names are listed below.\n";
        for (auto& pa : tables)
            outstream<<" "<<pa.first<<";\n";
    }
    void choose_command(istream& instream, ostream& outstream)
    {
        string name;
        instream>>name;
        if (!tables.count(name))
        {
            outstream<<"error: there is no table "<<name<<"\n";
            return;
        }
        is_table_chosen = true;
        chosen_table = name;
    }
    void rename_command(istream& instream, ostream& outstream)
    {
        string name1, name2;
        instream>>name1>>name2;
        if (!tables.count(name1))
        {
            outstream<<"error: there is no table "<<name1<<"\n";
            return;
        }
        if (tables.count(name2))
        {
            outstream<<"error: table "<<name2<<" already exists\n";
            return;
        }
        auto it = tables.extract(name1);
        it.key() = name2;
        tables.insert(move(it));
    }
    void delete_command(istream& instream, ostream& outstream)
    {
        string name;
        instream>>name;
        if (!tables.count(name))
        {
            outstream<<"error: there is no table "<<name<<"\n";
            return;
        }
        tables.erase(name);
    }

    void process_command(istream& instream, ostream& outstream)
    {
        if (is_table_chosen)
        {
            outstream<<"TABLE "<<chosen_table<<" >>> ";
            if (tables[chosen_table].process_command(instream, outstream))
                is_table_chosen = false;
            return;
        }
        else
            outstream<<"DATABASE >>> ";

        string command;
        instream>>command;
        if (command == "help")
            help(outstream);
        else if (command == "create")
            create_command(instream, outstream);
        else if (command == "view")
            view_command(outstream);
        else if (command == "choose")
            choose_command(instream, outstream);
        else if (command == "rename")
            rename_command(instream, outstream);
        else if (command == "delete")
            delete_command(instream, outstream);
        else
            outstream<<"error: incorrect command: "<<command<<"\n";
    }
};

Database db;

int main()
{
    while (true)
        db.process_command(cin, cout);
    return 0;
}
