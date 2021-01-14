#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;
 
int n_p_mos_counter = 1;
int wires_counter = 1;
string temp_wire; 
 
string NOT(string valid_expression, int location, int wires_counter, string a){
    //nmos first
    
    //id
    string not_implementation; 

    not_implementation = "M"; // + n_p_mos_counter + " ";
    not_implementation += to_string(n_p_mos_counter); 
    not_implementation += " ";
    //drain
    not_implementation += valid_expression[0];
    not_implementation += " ";
    //gate
    not_implementation += a;
    not_implementation += " ";
    //source and body
    not_implementation += "vdd vdd";
    //type
    not_implementation +=" PMOS \n";
 
    n_p_mos_counter++;
 
    not_implementation += "M"; 
    not_implementation += to_string(n_p_mos_counter);
    not_implementation += " ";
    //drain
    not_implementation += valid_expression[0];
    not_implementation += " ";
    //gate
    not_implementation += a;
    not_implementation += " ";
    //source and body
    not_implementation += "0 0";
    //type
    not_implementation += " NMOS \n";
 
    n_p_mos_counter++;

    return not_implementation;
}
 
 
string AND(string valid_expression, int location, int wires_counter, string a, string b){
    
    //M1
    string and_implementation; 

    and_implementation = "M"; 
    and_implementation += to_string(n_p_mos_counter); 
        //drain

    and_implementation += " W"; 
    and_implementation += to_string(wires_counter); 
    and_implementation += " ";
        //gate
 
    and_implementation += a;
        //source and body
    and_implementation += " 0 0";

        //type
    and_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire1 = wires_counter;   //common wire
 
    wires_counter++;
   
 
    //M2
    //drain
   
    and_implementation += "M"; 
    and_implementation += to_string(n_p_mos_counter); 
        //drain

    and_implementation += " W"; 
    and_implementation += to_string(wires_counter); 
    and_implementation += " ";
 
    //gate
    and_implementation += b;

        //source and body
    and_implementation += " W";
    and_implementation += to_string (common_wire1);
    and_implementation += " W";
    and_implementation += to_string (common_wire1);
        //type
    and_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire2 = wires_counter;
    string to_not = "W"; 
    to_not += to_string(common_wire2);
 
    wires_counter++;
 
 
    //M3
    and_implementation += "M";
    and_implementation += to_string (n_p_mos_counter);
    and_implementation += " W";
    and_implementation += to_string (common_wire2);
    and_implementation += " ";
        //gate
    and_implementation += b;
    and_implementation += " vdd vdd ";
        //type
    and_implementation += "PMOS \n";
 
    n_p_mos_counter++;
 
    wires_counter++;
 
 
 
    //M4
    and_implementation += "M";
    and_implementation += to_string(n_p_mos_counter);      
    n_p_mos_counter++;
        //drain
    and_implementation += " W";
    and_implementation += to_string (common_wire2);
    and_implementation += " ";
        //gate
    and_implementation += a;
        //source and body
    and_implementation += " vdd vdd ";
        //type
    and_implementation += "PMOS \n";
 
    //call not gate
    //string valid_expression, int location, int wires_counter, int i

    and_implementation += NOT(valid_expression, location, wires_counter, to_not);
   // cout << and_implementation;
    return and_implementation;
 }
 
 
string OR(string valid_expression, int location, int wires_counter, string a, string b)
{
     string or_implementation = "";
 
    //M1
    

   	or_implementation = "M";
	or_implementation += to_string(n_p_mos_counter);
        //drain
    	or_implementation +=" W";
	or_implementation += to_string(wires_counter);
	or_implementation += " ";
        //gate
	or_implementation += a;
	or_implementation += " ";
    
        //source and body
	or_implementation += "0 0";
  
        //type
	or_implementation += " NMOS \n";
  
 
    n_p_mos_counter++;
 
    int common_wire1 = wires_counter;   //common wire
 
    wires_counter++;
    //M2
   	or_implementation += "M";
	or_implementation += to_string(n_p_mos_counter);
        //drain
    	or_implementation +=" W";
	or_implementation += to_string(common_wire1);
	or_implementation += " ";
        //gate
	or_implementation += b;
	or_implementation += " ";
    
        //source and body
	or_implementation += "0 0";
  
        //type
	or_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire2 = wires_counter;

    string to_not = "W"; 
    to_not += to_string(common_wire2);

    wires_counter++;
//M3
   	or_implementation += "M";
	or_implementation += to_string(n_p_mos_counter);
        //drain
    	or_implementation +=" W";
	or_implementation += to_string(common_wire1);
	or_implementation += " ";
        //gate
	or_implementation += a;
	or_implementation += " ";
    
        //source and body
	or_implementation += "W";
	or_implementation += to_string(common_wire2);
	or_implementation += " W";
	or_implementation += to_string(common_wire2);
  
        //type
	or_implementation += " PMOS \n";
 

 
    n_p_mos_counter++;
 
    wires_counter++;
 
     //M4
   	or_implementation += "M";
	or_implementation += to_string(n_p_mos_counter);
        //drain
    	or_implementation +=" W";
	or_implementation += to_string(common_wire2);
	or_implementation += " ";
        //gate
	or_implementation += b;
	or_implementation += " ";
    
        //source and body
	or_implementation += "vdd vdd";
  
        //type
	or_implementation += " PMOS \n";

 	n_p_mos_counter++;
	or_implementation += NOT(valid_expression, location, wires_counter, to_not);
	return or_implementation;
}
 
 
 
 
 
 
 
 
void generate(string valid_expression)
{
    ofstream NetList;
    NetList.open("netlist.txt", ios::out);
    int location = 0;
    location = valid_expression.find('=');
   
    NetList << valid_expression << '\n';
 
    // should detect the precednce
    for(int i = location + 1; i<valid_expression.length(); i++)
    {
        if(valid_expression[i+1] == '`')
        {
            if(isalpha(valid_expression[i]))
            {
                //call NOT function
                string a;
                a += valid_expression[i];
                NetList << NOT(valid_expression, location, wires_counter, a);
       
            }
        }
    }
    for(int i = location + 1; i<valid_expression.length(); i++)
    {
        if(valid_expression[i+1] == '&')
        {
            if(isalpha(valid_expression[i]))
            {
                //call AND function
                string a;
                a += valid_expression[i];
                string b;
                b += valid_expression[i+2];
                NetList << AND(valid_expression, location, wires_counter, a, b);
       
            }
        }
    }

    for(int i = location + 1; i<valid_expression.length(); i++)
    {
        if(valid_expression[i+1] == '|')
        {
            if(isalpha(valid_expression[i]))
            {
                //call OR function
                string a;
                a += valid_expression[i];
                string b;
                b += valid_expression[i+2];
                NetList << OR(valid_expression, location, wires_counter, a, b);
       
            }
        }      
 
       
}  
    NetList << "END\n";
    NetList.close();   
    return;
}
 
 
bool valid(string expression)
{
    char out = expression[0];
    int location = 0;
    location = expression.find('=');
 
    for(int i = location+1; i<expression.length(); i++)
    {
        if(expression[i] == out)
        {
            return false;
        }
       
        if(expression[i] != '|' && expression[i] != '&' && expression[i] != '`' && expression[i]!=' ')
        {
            if(!isalpha(expression[i]))
            {      
                return false;
            }
        }
    }
   
    return true;
}
 
 
void handler()
{
    string boolean_expression;
    cout<<"\nPlease, enter your boolean expression: ";
    getline(cin, boolean_expression);
   
    if( valid(boolean_expression) )
    {
        cout <<"You entred: " << boolean_expression << endl;
        generate(boolean_expression);
        return;
    }
   
    else
    {
        cout<<"\ninvalid input."<<endl;
        handler();
    }
}
 
 
 
int main()
{
   
    cout<<"\nHello, this program generates NetSpice code for your boolean function."<<endl;
   
    handler();
   
   
    return 0;
}
