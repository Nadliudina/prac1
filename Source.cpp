
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <iterator>
using namespace std;

typedef struct field {
    string name;
    string val;
    //friend ostream& operator << (ostream& s, field& my)
    //{
    //    return s << "name= " << my.name << " val= " << my.val << endl;
    //}
};
typedef struct node {
    string name;
    int id;
    int pid;
    list <field> fields;
    list <node> child;
};
//на экран рекурсивно
void print_node(node& x, int tabCount) {
    for (int e = 0; e < tabCount; e++)
        cout << "   ";
    cout << "name: " << x.name << endl;
    for (int e = 0; e < tabCount; e++)
        cout << "   ";
    cout << "id: " << x.id << endl;
    for (int e = 0; e < tabCount; e++)
        cout << "   ";
    cout << "pid: " << x.pid << endl;
    list<field>::iterator i;
    list<node>::iterator j;

    for (i = x.fields.begin(); i != x.fields.end(); i++)
    {
        for (int e = 0; e < tabCount; e++)
            cout << "   ";
        cout << i->name << ": ";
        cout << i->val << endl;
    }
    for (int e = 0; e < tabCount; e++)
        cout << "   ";
    cout << "child_ids: ";
    for (j = x.child.begin(); j != x.child.end(); j++)
    {

        cout << j->id << ", ";
    }
    cout << endl;
    for (j = x.child.begin(); j != x.child.end(); j++)
    {
        print_node(*j, tabCount + 1);
    }
}
 string strout="";
 //на файл рекурсивно
void print_node_in_file(node& x, int tabCount,string file) {
  
    ofstream of;
    of.open(file);
   
    for (int e = 0; e < tabCount; e++)
         strout+= "   ";
    strout+= "name: " + x.name + "\n";
    for (int e = 0; e < tabCount; e++)
        strout+= "   ";
    strout += "id: " + to_string(x.id); strout += "\n";
    for (int e = 0; e < tabCount; e++)
        strout+= "   ";
    strout += "pid: " + to_string(x.pid); strout += "\n";
    list<field>::iterator i;
    list<node>::iterator j;

    for (i = x.fields.begin(); i != x.fields.end(); i++)
    {
        for (int e = 0; e < tabCount; e++)
            strout+= "   ";
        strout+= i->name + ": ";
        strout+= i->val + "\n";
    }
    for (int e = 0; e < tabCount; e++)
        strout+= "   ";
    strout+= "child_ids: ";
    for (j = x.child.begin(); j != x.child.end(); j++)
    {
        strout+= to_string(j->id) + ", ";
    }
    strout+= "\n";
    for (j = x.child.begin(); j != x.child.end(); j++)
    {
        print_node_in_file(*j, tabCount + 1,file);
    }
    of << strout;
}
//id
int count1 = 0;
//главный рекурсивный процесс
int parse(int len,int curI,const char* char_str,node * curNode,int parNode) {
     cout << "COUTN " << count1 << endl;
    
    for (int i = curI; i < len; i++)
    {
       cout << "i,x " << i << "," << char_str[i] <<endl;
        cout << "____________\n";
        bool fields_end = false;
        string word = "";
        while (char_str[i] != '=') {
            word += char_str[i];         i++;
        }
        cout << "NAME " << word << endl;
        curNode->name = word;
        curNode->id = count1;
        count1++;
        curNode->pid = parNode;
        if (char_str[i] != '=') {
            cout << "ERROR1 (" << i << ")" << endl;
        }
        i++;
        if (char_str[i] != '{') {
            cout << "ERROR2 (" << i << ")" << endl;
            cout << char_str[i - 1] << char_str[i] << char_str[i + 1] << endl;
        }else
        i++;

        /////////////////////////////fields
        while (!fields_end)
        {
            word = "";
            if (char_str[i] == '}') {
                fields_end = true; break;
            }
            int memi = i;//начало нового объекта
            while (char_str[i] != '=') {
                word += char_str[i];         i++;
            }
            string temp_word;
            temp_word = word;
            word = "";
            if (char_str[i] != '=') {
                cout << "ERROR3 (" << i << ")" << endl;
            }
            i++;
            if (char_str[i] != '"') {
                if (char_str[i] == '{')//child 
                {
                    cout << "+++";
                    node* temp = new node();
                    curNode->child.push_back(*(new node()));
                    curNode->child.back().name = word;
                   i= parse(len, memi,char_str, &(curNode)->child.back(),curNode->id);
                }else
                cout << "ERROR4 (" << i << ")" << endl;
            }
            else
            {
                i++;
                while (char_str[i] != '"') {
                    word += char_str[i];         i++;
                }
                i++;
                curNode->fields.push_back({ temp_word,word });
            }
        
        }
        //////////////////////////////////////fields
        return i+1;//где остановились
    }
}

int main(int argc, char* argv[])
{
    if (argc!=3)
    {
        cout << "argc!=3" << endl;
        cout << "argc=" <<argc<< endl;
        return 0;
    }
    string str,str1;
 
    node Node; int id = 0;
    ifstream ifs;
    ifs.open(argv[1]);
    while (!ifs.eof())
    {
        ifs >> str1;
        str += str1;
    }
    ifs.close();
    const char* char_str = str.c_str();
    for (int i = 0; i < str.length(); i++) {
        cout << char_str[i];
    }
    cout << "SIZE = " << str.length() << endl;

    parse(str.length(),0, char_str,&Node,0);

   
    print_node(Node, 0);
    print_node_in_file(Node, 0, argv[2]);
    return 0;
  //  node a,b,c;
  //  a.name = "meme";
  //  a.id = 0;
  //  a.pid = 0;
  //  a.fields.push_back({ "type", "test_type" });
  //  a.fields.push_back({ "land", "england" });
 //   print_node(a);
    //b.name = "memeB";
    //b.id = 1;
    //b.pid = 0;
    //b.fields.push_back({ "type", "test_typeB" });
    //b.fields.push_back({ "land", "englanBBBd" });
    //
    //c.name = "memeC";
    //c.id = 2;
    //c.pid = 0;
    //c.fields.push_back({ "type", "test_typeCCC" });
    //c.fields.push_back({ "land", "englanCCCC" });

    //a.child.push_back(b);
    //a.child.push_back(c);

   
    //ofstream of;
    //of.open(argv[2]);
    //of << strout;
    //cout << strout;
 
}