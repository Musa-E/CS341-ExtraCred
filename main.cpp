#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;
int random_number(int min, int max){
	srand(time(NULL));
	return rand() % max + min;
}

int main(int argc, char *argv[]){
	ifstream rf;
	rf.open(argv[1], ifstream::in);

	ofstream wf;
	wf.open("output.cpp", std::ofstream::out | std::ofstream::trunc);
	//Check for file opening errors
	if (rf.fail() || wf.fail()){
		cerr << endl << "Error Opening File" << endl;
		rf.close();
		wf.close();
		exit(1);
	}
	//Read file
	string tokens[100];
	int current_token=0;

	char ch;
	string tok;

	int string_state=0;
	string t_string;

	int comment_state=0;
	while (rf >> std::noskipws >> ch){ //std::noskipws make it where we can see spaces
		tok += ch;
		if (string_state){
			t_string += tok;
			tok = "";
		}

		if (ch == '\n' || ch == '	'){//turns new lines into spaces
			ch = ' ';
		}

		if (ch == '\"'){
			string_state = !string_state;
			if (!string_state){
				tokens[current_token] = t_string;
				current_token++;
				tok = "";
				t_string = "";
			}
		}
		else if (ch == ' ' && !string_state){
			tok.pop_back();
			//Edit token
			if (tok == "//"){
				comment_state = !comment_state;
				tok = "";
			}
			else if (tok == ""){
				continue;
			}
			//Add token
			if (!comment_state && tok != ""){
				tokens[current_token] = tok;
				current_token++;
			}
			tok = "";
		}
	}
	tokens[current_token] = tok;
	current_token++;
	//Read tokens
	string civ_options[7] = {"Drugs are bad!", "Doing crimes will give you time.", "Subscribe to Z1ngg!", "Truttle1 is cool!", "The programmer is a nerd.", "You smell bad!", ":>"};
	wf << "#include <iostream>\n#include <string>\n\nusing namespace std;\n";
	int heat=0;
	string token;
	for (int i=0; i<current_token; i++){
		token = tokens[i];
		//cout << token << ", ";
		if (heat >= 10){
			cerr << endl << "You got caught! (heat to high)" << endl;
			break;
		}
		else if (heat < 0){
			cerr << endl << "You became a good citizen and stopped going to the Back Alleys! (heat to low)" << endl;
			break;
		}

		if (token == "say"){
			if (heat > 1){heat++;}
			int check_plus=2;
			while (tokens[i+check_plus] == "+"){
				tokens[i+1] = tokens[i+1] + " << " + tokens[i+check_plus+1];
				check_plus += 2;
			}
			wf << endl << "cout << " << tokens[i+1] << ";";
		}
		else if (token == "buy"){
			heat++;
			wf << endl << tokens[i+1] << " " << tokens[i+2] << " = " << tokens[i+3] << ";";
		}
		else if (token == "add"){
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << " + " << tokens[i+3] << ";";
		}
		else if (token == "subtract"){
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << " - " << tokens[i+3] << ";";
		}
		else if (token == "multiply"){
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << " * " << tokens[i+3] << ";";
		}
		else if (token == "divide"){
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << " / " << tokens[i+3] << ";";
		}
		else if (token == "remainder"){
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << " % " << tokens[i+3] << ";";
		}
		else if (token == "mug"){
			heat++;
			wf << endl << "cin >> " << tokens[i+1] << ";";
		}
		else if (token == "heat"){
			wf << endl << "cout << " << "\"Heat: \" << "<< heat << ";";
		}
		else if (token == "hide"){ //Sleep takes microseconds which are 1/1000 of a Second
			heat -= stoi(tokens[i+1]);
			//wf << endl << "Sleep(" << tokens[i+1] << "*1000);";
		}
		else if (token == "crime"){
			heat += stoi(tokens[i+1]);
			//wf << endl << "Sleep(" << tokens[i+1] << "*1000);";
		}
		else if (token == "ask"){
			if (tokens[i+1] == "person"){
				int rnd_num = random_number(0, 3);
				if (rnd_num == 0){
					tokens[i+1] = "civillian";
				}
				else if (rnd_num == 1){
					tokens[i+1] = "cop";
				}
				else if (rnd_num == 2){
					tokens[i+1] = "criminal";
				}
			}
			string if_reqs = tokens[i+2] + tokens[i+3] + tokens[i+4];
			if (tokens[i+5] == "and"){
				if_reqs += " && " + tokens[i+6] + tokens[i+7] + tokens[i+8];
			}
			else if (tokens[i+5] == "or"){
				if_reqs += " || " + tokens[i+6] + tokens[i+7] + tokens[i+8];
			}

			if (tokens[i+1] == "civillian"){
				wf << endl << "if (" << if_reqs << "){";
				wf << endl << "cout << \"Civillian - " << civ_options[random_number(0,6)] <<"\" << endl;";
			}
			else if (tokens[i+1] == "cop"){
				if (heat < 3){
					heat--;
					wf << endl << "if (" << if_reqs << "){";
				}
				else {
					cerr << endl << "You got arrested! (heat to high, asked cop)";
				}
			}
			else if (tokens[i+1] == "criminal"){
				if (heat > 3){
					wf << endl << "if (" << if_reqs << "){";
				}
				else {
					cerr << endl << "You got jumped! (heat to low, asked criminal)";
				}
			}
			else if (tokens[i+1] == "another"){
				if (tokens[i+2] == "person"){
					int rnd_num = random_number(0, 3);
					if (rnd_num == 0){
						tokens[i+2] = "civillian";
					}
					else if (rnd_num == 1){
						tokens[i+2] = "cop";
					}
					else if (rnd_num == 2){
						tokens[i+2] = "criminal";
					}
				}
				string if_reqs = tokens[i+3] + tokens[i+4] + tokens[i+5];
				if (tokens[i+6] == "&&"){
					if_reqs += " && " + tokens[i+7] + tokens[i+8] + tokens[i+9];
				}
				else if (tokens[i+6] == "or"){
					if_reqs += " || " + tokens[i+7] + tokens[i+8] + tokens[i+9];
				}

				if (tokens[i+2] == "civillian"){
					wf << endl << "else if (" << tokens[i+3] << tokens[i+4] << tokens[i+5] << "){";
					wf << endl << "cout << \"Civillian - " << civ_options[random_number(0,6)] <<"\" << endl;";
				}
				else if (tokens[i+2] == "cop"){
					if (heat < 3){
						heat--;
						wf << endl << "else if (" << tokens[i+3] << tokens[i+4] << tokens[i+5] << "){";
					}
					else {
						cerr << endl << "You got arrested! (heat to high, asked cop)";
					}
				}
				else if (tokens[i+2] == "criminal"){
					if (heat > 3){
						wf << endl << "else if (" << tokens[i+3] << tokens[i+4] << tokens[i+5] << "){";
					}
					else {
						cerr << endl << "You got jumped! (heat to low, asked criminal)";
					}
				}
			}
			else if (tokens[i+1] == "no-one"){
				wf << endl << "else {";
			}
			tokens[i+1] = "person";
		}
		else if (token == "skip"){
			wf << endl << "continue;";
		}
		else if (token == "end"){
			wf << endl << "break;";
		}
		else if (token == "stop"){
			if (tokens[i+1] == "asking" || tokens[i+1] == "tripping"){
				wf << endl << "}";
			}
		}
		else if (token == "trip"){
			heat++;
			wf << endl << "for (int " << tokens[i+1] << "=0; " << tokens[i+1] << "<" << tokens[i+2] << "; " << tokens[i+1] << "++){";
		}
		else if (token == "whisper"){
			for(unsigned int x=0;x<tokens[i+1].length();x++){
					tokens[i+1][x] = tolower(tokens[i+1][x]);
				}
			wf << endl << "cout << " << tokens[i+1] << ";";
		}
		else if (token == "use"){
			heat++;
			wf << endl << tokens[i+1] << " = " << tokens[i+2] << ";";
		}
		else if (token == "hire"){
			if (tokens[i+1] == "yourself"){
				tokens[i+1] = "main";
			}
			else if (tokens[i+1] == "main"){
				cerr << endl << "Cannot hire \"main\"";
			}

			string function_inputs;
			int check_input=3;
			while (tokens[i+check_input] == "task"){
				function_inputs += tokens[i+check_input+1] + " " + tokens[i+check_input+2] + ",";
				check_input += 3;
			}
			if (function_inputs != ""){
				function_inputs.pop_back();
			}

			wf << endl << tokens[i+2] << " " << tokens[i+1] << "(" << function_inputs << "){";
		}
		else if (token == "pay"){
			wf << endl << "return " << tokens[i+1] << ";" << endl << "}" << endl;
		}
		else if (token == "call"){
			string function_inputs="";
			int check_input=2;
			while (tokens[i+check_input] != "hang-up"){
				function_inputs += tokens[i+check_input] + ",";
				check_input++;
			}
			if (function_inputs != ""){
				function_inputs.pop_back();
			}
			wf << endl << tokens[i+1] << "(" << function_inputs << ");";
		}
	}
	//Close Files
	rf.close();
	wf.close();
	//Compile C++ file
	system("g++ output.cpp -o output.exe");
	//Delete C++ file
	if (remove("output.cpp")==0){
		cout << "Finished!" << endl << endl;
	}
	else{
		cout << "Error";
	}
	cout << "Output: " << endl;
	system("output.exe");

	return 0;
}
