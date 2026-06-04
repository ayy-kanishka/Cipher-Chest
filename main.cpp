#include<iostream>
#include<string>
#include<sstream>
#include<cctype>
#include<map>
#include<vector>
#include<stack>
#include<set>
#include<utility>
#include<numeric>
#include<limits>
#include<fstream>
using namespace std;
class Cipher{
    protected:
    string message;
    public:
    Cipher(string msg) : message(msg){}
    virtual string encrypt() = 0;
    virtual string decrypt() = 0;
};
class ASCIICipher : public Cipher{
    private:
    int shift;
    public:
    ASCIICipher(string msg) : Cipher(msg){
        shift=3;
    }
    ASCIICipher(string msg, int s) : Cipher(msg){
        shift=s;
    }
    string encrypt()override{
        string result=message;
        for(int i=0; i<result.length(); i++){
            result[i]=result[i]+shift;
        }
        cout<<"Encrypted message : "<<endl;
        return result;
    }
    string decrypt()override{
        string result=message;
        for(int i=0; i<result.length(); i++){
            result[i]=result[i]-shift;
        }
        cout<<"Decrypted message : "<<endl;
        return result;
    }
};
class MirrorCipher : public Cipher{
    public:
    MirrorCipher(string msg) : Cipher(msg){}
    string encrypt() override{
        stack<char> s;
        for(char ch : message){
            s.push(ch);
        }
        string result = "";
        while(!s.empty()){
            result += s.top();
            s.pop();
        }
        return result;
    }
    string decrypt() override{
        return encrypt();
    }
};
class MorseCipher : public Cipher {
private:
    map<char, string> morse;
    map<string, char> revmorse;
public:
    MorseCipher(string msg) : Cipher(msg) {
        morse['A'] = ".-";    morse['B'] = "-...";  morse['C'] = "-.-.";
        morse['D'] = "-..";   morse['E'] = ".";     morse['F'] = "..-.";
        morse['G'] = "--.";   morse['H'] = "....";  morse['I'] = "..";
        morse['J'] = ".---";  morse['K'] = "-.-";   morse['L'] = ".-..";
        morse['M'] = "--";    morse['N'] = "-.";    morse['O'] = "---";
        morse['P'] = ".--.";  morse['Q'] = "--.-";  morse['R'] = ".-.";
        morse['S'] = "...";   morse['T'] = "-";     morse['U'] = "..-";
        morse['V'] = "...-";  morse['W'] = ".--";   morse['X'] = "-..-";
        morse['Y'] = "-.--";  morse['Z'] = "--..";
        morse['1'] = ".----"; morse['2'] = "..---"; morse['3'] = "...--";
        morse['4'] = "....-"; morse['5'] = "....."; morse['6'] = "-....";
        morse['7'] = "--..."; morse['8'] = "---.."; morse['9'] = "----.";
        morse['0'] = "-----"; morse[' '] = "/";     
        for (auto const& [key, val] : morse) {
            revmorse[val] = key;
        }
    } 
    string encrypt() override {
        string result = "";
        for (char ch : message) {
            ch = toupper(ch);
            if (morse.find(ch) != morse.end()) {
                result += morse[ch] + " "; 
            }
        }
        if (!result.empty()){
            result.pop_back();
        } 
        return result;
    }
    string decrypt() override {
        string result = "";
        string code;
        stringstream ss(message);
        while (ss >> code) {
            if (code == "/") {
                result += " "; 
            } else if (revmorse.find(code) != revmorse.end()) {
                result += revmorse[code];
            }
        }
        return result;
    }
};
class CaesarCipher : public Cipher {
    private:
    int shift;
    public:
    CaesarCipher(string msg) : Cipher(msg), shift(3){}
    string encrypt() override{
        string result = "";
        for(char ch : message){
            if(isupper(ch)) {
                result += char(((ch - 'A' + shift) % 26) + 'A');
            } else if(islower(ch)) {
                result += char(((ch - 'a' + shift) % 26) + 'a');
            } else {
                result += ch;
            }
        }
        return result;
    }
    string decrypt() override{
        string result = message;
        for(int i = 0; i < result.length(); i++){
            char ch = result[i];
            if(isupper(ch)){
                result[i] = ((ch - 'A' - shift + 26) % 26) + 'A';
            } else if(islower(ch)) {
                result[i] = ((ch - 'a' - shift + 26) % 26) + 'a';
            }
        }
        return result;
    }
};
class AtbashCipher : public Cipher{
    public:
    AtbashCipher(string msg) : Cipher(msg){}
    string encrypt() override{
        string result = "";
        for(char ch : message){
            if(isupper(ch)){
                result += char('Z'-(ch-'A'));
            } else if(islower(ch)){
                result += char('z'-(ch-'a'));
            } else {
                result += ch;
            }
        }
        return result;
    }
    string decrypt() override{
        return encrypt();
    }
};
class VigenereCipher : public Cipher{
    private:
    string key;
    public:
    VigenereCipher(string msg, string k) : Cipher(msg), key(k) {}
    string encrypt() override{
        string result = "";
        for(int i=0; i<message.length(); i++){
            char ch = message[i];
            if(isupper(ch)){
                int shift = toupper(key[i % key.length()])-'A';
                result += char(((ch-'A' + shift)%26)+'A');
            } else if(islower(ch)) {
                int shift = tolower(key[i % key.length()])-'a';
                result += char(((ch-'a' + shift)%26)+ 'a');
            } else {
                result += ch;
            }
        }
        return result;
    }
    string decrypt() override{
        string result = "";
        for(int i=0; i<message.length(); i++){
            char ch = message[i];
            if(isupper(ch)){
                int shift = toupper(key[i % key.length()])-'A';
                result += char(((ch-'A' - shift + 26)%26)+'A');
            } else if(islower(ch)){
                int shift = tolower(key[i % key.length()])-'a';
                result += char(((ch-'a' - shift + 26)%26)+'a');
            } else {
                result +=ch;
            }
        }
        return result;
    }
};
class AffineCipher : public Cipher{
    private:
    int a;
    int b;
    public:
    AffineCipher(string msg, int x, int y) : Cipher(msg), a(x), b(y){}
    bool isValid(){
        if(std::gcd(a,26)!=1){
            return false;
        } else {
            return true;
        }
    }
    string encrypt() override{
        string result = "";
        for(char ch : message){
            if(!isValid()){
                cout<<"invalid char";
                return "";
            } else {
                if(isupper(ch)){
                    int x = ch-'A';
                    int enc = (a*x + b)%26;
                    result += enc + 'A';
                } else if(islower(ch)){
                    int x = ch-'a';
                    int enc = (a*x + b)%26;
                    result += enc + 'a';
                } else {
                    result += ch;
                }
            }
        }
        return result;
    }
    int modInv() {
        for(int i=1; i<26; i++)
        {
            if((a*i)%26 == 1) {
                return i;
            }
        }
        return -1;
    }
    string decrypt() override{
        int aInv = modInv();
        string result;
        for(char ch : message){
            if(!isValid()){
                cout << "Invalid value of a. gcd(a,26) must be 1";
            } else {
                if(isupper(ch)){
                    int x = ch-'A';
                    int dec = (aInv*(x-b+26))%26;
                    result += dec + 'A';
                } else if(islower(ch)){
                    int x = ch-'a';
                    int dec = (aInv*(x-b+26))%26;
                    result += dec + 'a';
                } else {
                    result += ch;
                }
            }
        }
        return result;
    }
}; 
class StreamCipher : public Cipher{
    private:
    string key;
    public:
    StreamCipher(string msg, string k) : Cipher(msg), key(k){}
    string encrypt() override{
        string result = "";
        for(int i=0; i<message.length(); i++){
            result += char(message[i]^key[i % key.length()]);
        }
        return result;
    }
    string decrypt() override{
        return encrypt();
    }
};
class CustomCipher : public Cipher{
    private:
    map<char, char> mapping;
    map<char, char> revMapping;
    public:
    CustomCipher(string msg, map<char, char> m) : Cipher(msg){
        mapping=m;
        for(auto pair : mapping){
            revMapping[pair.second]=pair.first;
        }
    }
    string encrypt() override{
        string result="";
        for(char ch : message){
            if(mapping.count(ch)){
                result += mapping[ch];
            } else {
                result += ch;
            }
        }
        return result;
    }
    string decrypt() override{
        string result ="";
        for(char ch : message){
            if(revMapping.count(ch)){
                result += revMapping[ch];
            } else {
                result += ch;
            }
        }
        return result;
    }
};
class Node{
    public:
    char data;
    Node *next, *prev;
    Node(char d){
        data=d;
        next=NULL;
        prev=NULL;
    }
};
class ChainCipher : public Cipher{
    private:
    Node* head;
    int step;
    public:
    ChainCipher(string msg, int s) : Cipher(msg){
        step = s;
        head=NULL;
    }
    ~ChainCipher(){
        if(head == nullptr) {
            return;
        }
        Node* current = head->next;
        while(current != head) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        delete head;
    }
    void insert(char ch){
        Node* newnode = new Node(ch);
        if(head == NULL){
            head = newnode;
            head->prev = head;
            head->next = head;
            return;
        }
        Node *tail = head->prev;
        tail->next = newnode;
        newnode->prev = tail;
        newnode->next = head;
        head->prev = newnode;
    }
    Node *findNode(char ch){
        if(head == NULL){
            return NULL;
        }
        Node *temp = head;
        do{
            if(temp-> data == ch){
                return temp;
            }
            temp = temp->next;
        } while(temp != head);
        return NULL;
    }
    string encrypt() override{
        string result = "";
        for(char ch : message){
            Node* current = findNode(ch);
            if(current){
                Node *temp = current;
                for(int i=0; i<step; i++){
                    temp = temp->next;
                }
                result += temp->data;
            } else {
                result += ch;
            }
        }
        return result;
    }
    string decrypt() override{
        string result = "";
        for(char ch : message){
            Node *current = findNode(ch);
            if(current){
                Node *temp = current;
                for(int i=0; i<step; i++){
                    temp = temp->prev;
                }
                result += temp->data;
            } else {
                result += ch;
            }
        }
        return result;
    }
};
void displayMenu(){
    cout<<"ENTER CHOICE OF CRYPTOGRAPHY : "<<endl;
    cout<<"1. ASCII CIPHER \n2. MIRROR CIPHER \n3. MORSE CIPHER \n4. CEASER CIPHER \n5. ATBASH CIPHER \n6. VIGENERE CIPHER \n7. AFFINE CIPHER \n8. STREAM CIPHER \n9. CUSTOM CIPHER(smwht like ascii and ceaser) \n10. CUSTOM CHAIN CIPHER(custom cipher but in CDLL) \n11. VIEW HISTORY \n\n0. EXIT \nENTER : ";
}
string performOperation(Cipher &c){
    char op;
    cout<<"ENCRYPT OR DECRYPT? (e/d) : ";
    cin>>op;
    if(op=='e'){
        return c.encrypt();
    } else if(op=='d'){
        return c.decrypt();
    } else{
        cout<<"Invalid option"<<endl;
        return "";
    }
}
string getMessage(){
    string msg;
    cout<<"ENTER MESSAGE : "<<endl;
    cin.ignore(numeric_limits<streamsize>:: max(), '\n');
    getline(cin, msg);
    return msg;
}
void SaveFile(string name, string msg, string result) {
    ofstream file("saved.txt", ios::app);
    if(file.is_open()){
        file<<"CIPHER : "<<name<<endl;
        file<<"ORIGINAL MESSAGE : "<<msg<<endl;
        file<<"RESULT : "<<result<<endl;
        file<<"-----------------------------------"<<endl;
        file.close();
        cout<<"DATA SAVED SUCCESSFULLY!"<<endl;
    } else{
        cout<<"ERROR OPENING FILE!"<<endl;
    }
}
void viewHistory() {
    ifstream file("saved.txt");
    string line;
    while(getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}
int main(){
    int choice;
    do{
        displayMenu();
        cin>>choice;
        switch(choice){
            case 1: {
                string msg = getMessage();
                int shift;
                string result;
                cout<<"ENTER SHIFT : ";
                cin>>shift;
                ASCIICipher a(msg, shift);
                result = performOperation(a);
                SaveFile("ASCII CIPHER", msg, result);
            }
            break;
            case 2:{
                string msg = getMessage();
                MirrorCipher m(msg);
                string result;
                result = performOperation(m);
                SaveFile("MIRROR CIPHER", msg, result);
            }
            break;
            case 3:{
                string msg = getMessage();
                MorseCipher m(msg);
                string result;
                result = performOperation(m);
                SaveFile("MORSE CIPHER", msg, result);
            }
            break;
            case 4:{
                string msg = getMessage();
                CaesarCipher c(msg);
                string result;
                result = performOperation(c);
                SaveFile("CAESAR CIPHER", msg, result);
            }
            break;
            case 5:{
                string msg = getMessage();
                AtbashCipher a(msg);
                string result;
                result = performOperation(a);
                SaveFile("ATBASH CIPHER", msg, result);
            }
            break;
            case 6:{
                string msg = getMessage();
                string key;
                string result;
                cout<<"ENTER KEY : "<<endl;
                cin>>key;
                VigenereCipher v(msg, key);
                result = performOperation(v);
                SaveFile("VIGENERE CIPHER", msg, result);
            }
            break;
            case 7:{
                string msg = getMessage();
                int a, b;
                string result;
                cout<<"ENTER VALUE OF a : ";
                cin>>a;
                cout<<"ENTER VALUE OF b : ";
                cin>>b;
                AffineCipher ac(msg, a, b);
                result = performOperation(ac);
                SaveFile("AFFINE CIPHER", msg, result);
            }
            break;
            case 8:{
                string msg = getMessage();
                string key;
                string result;
                cout<<"ENTER KEY : ";
                cin>>key;
                StreamCipher s(msg, key);
                result = performOperation(s);
                SaveFile("STREAM CIPHER", msg, result);
            }
            break;
            case 9:{
                string msg = getMessage();
                map<char,char> mapping;
                int n;
                string result;
                cout<<"ENTER NUMBER OF MAPPINGS : ";
                cin>>n;
                for(int i=0; i<n; i++){
                    char from, to;
                    cout<<"ENTER CHARACTER TO BE REPLACED FROM : ";
                    cin>>from;
                    cout<<"ENTER CHARACTER TO BE REPLACED WITH : ";
                    cin>>to;
                    mapping[from]=to;
                }
                CustomCipher c(msg, mapping);
                result = performOperation(c);
                SaveFile("CUSTOM CIPHER", msg, result);
            }
            break;
            case 10:{
                string msg = getMessage();
                int step;
                string result;
                cout<<"ENTER STEP : ";
                cin>>step;
                ChainCipher c(msg, step);
                int n;
                cout<<"ENTER NUMBER OF CHARACTERS : ";
                cin>>n;
                for(int i=0; i<n; i++){
                    char ch;
                    cout<<"ENTER CHARACTER : ";
                    cin>>ch;
                    c.insert(ch);
                }
                result = performOperation(c);
                SaveFile("CHAIN CIPHER", msg, result);
            }
            break;
            case 11:{
                viewHistory();
            }
            break;
            case 0:{
                cout<<"EXITING...";
                return 0;
            }
            default:{
                cout<<"invalid choice";
            }
        }
    } while(choice!=0);
}