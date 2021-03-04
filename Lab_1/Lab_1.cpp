#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class PlainText {
protected:

    string plaintext = "";
    string chipher = "";
    string decryptedtext = "";

public:
    void add_plaintext(string filename)
    {
        ifstream plaintext_file(filename);

        if (!plaintext_file.is_open())
        {
            cout << "File was not open";
        }
        else
        {
            string tmp;
            while (getline(plaintext_file, tmp))
                plaintext += tmp + '\n';
            plaintext.erase(plaintext.begin() + plaintext.length() - 1);

            plaintext_file.close();
        }
    }

    void print_plaintext()
    {
        cout << plaintext << endl;
    }

    void print_chipher()
    {
        cout << chipher << endl;
    }

    void print_decryptedtext()
    {
        cout << decryptedtext << endl;
    }
};

class ColumnCipher : public PlainText {
private:
    string key;

    void enter_key() {
        cout << "Enter key: ";
        cin >> key;
    }

    vector<int> generate_key()
    {
        int column_number = key.length();         
        vector<int> tmp_key(column_number, 0);

        for (int i = 0; i < column_number; i++)
        {
            char c = key[i];
            for (int j = 0; j < column_number; j++)
                if ((c > key[j]) || (c == key[j] && i > j))
                    tmp_key[i]++;
        }

        vector<int> res_key(column_number);

        for (int i = 0; i < column_number; i++)
            for (int j = 0; j < column_number; j++)
            {
                if (tmp_key[j] == i)
                    res_key[i] = j;
            }

       /* for (int i = 0; i < column_number; i++) {
            cout << key[i] << " - " << res_key[i] << endl;
        }
       */

        return res_key;
    }

public:
    
    void encrypt_data()
    {
        enter_key();
        vector<int> vec_key = generate_key();

        int columns = vec_key.size();
        int lines = ceil((float)plaintext.length() / (float)columns);

        //cout << "colums = " << columns << " lines = " << lines;

        vector<vector<char>> data(lines, vector<char> (columns));

        int data_counter = 0;
        for (int i = 0; i < lines; i++)
            for (int j = 0; j < columns; j++)
            {
                if (data_counter < plaintext.length())
                    data[i][j] = plaintext[data_counter++];
                else
                    data[i][j] = (i + j) % 26 + 'a';
            }

        /* for (int i = 0; i < lines; i++)
        {
            for (int j = 0; j < columns; j++)
                cout << data[i][j] << " ";
            cout << endl;
        }
        */

        for (int j = 0; j < columns; j++)
            for (int i = 0; i < lines; i++)
                chipher += data[i][vec_key[j]];
        
    }

    void decrypt_data()
    {
        vector<int> vec_key = generate_key();

        int columns = vec_key.size();
        int lines = chipher.length() / columns;

        //cout << "colums = " << columns << " lines = " << lines;

        vector<vector<char>> decryption(lines, vector<char>(columns));

        int chipher_counter = 0;
        for (int j = 0; j < columns; j++)
            for (int i = 0; i < lines; i++)
                decryption[i][vec_key[j]] = chipher[chipher_counter++];
     
        for (int i = 0; i < lines; i++)
            for (int j = 0; j < columns; j++)
                decryptedtext += decryption[i][j];
            
        int extra_chars = columns - (plaintext.length() % columns);
        decryptedtext.erase(decryptedtext.end() - extra_chars, decryptedtext.end());
    }
};


int main()
{
    ColumnCipher s;
    s.add_plaintext("plaintext.txt");
    s.encrypt_data();
    s.print_chipher();
    s.decrypt_data();
    s.print_decryptedtext();
}