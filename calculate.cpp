#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <vector>

using namespace std;

int get_behavior_val(string feeling) {
    if (feeling == "Strongly Disagree") {
        return 1;
    }
    else if (feeling == "Somewhat Disagree") {
        return 2;
    }
    else if (feeling == "I am Unsure" || feeling == "I am unsure") {
        return 3;
    }
    else if (feeling == "Somewhat Agree") {
        return 4;
    }
    else if (feeling == "Strongly Agree") {
        return 5;
    }
    return 99999;
}

int get_feeling_val(string feeling) {
    if (feeling == "Definitely") {
        return 5;
    }
    else if (feeling == "Probably") {
        return 4;
    }
    else if (feeling == "Not Sure") {
        return 3;
    }
    else if (feeling == "Probably Not") {
        return 2;
    }
    else if (feeling == "Definitely Not") {
        return 1;
    }
    return 99999;
}

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    //get rid of makefile error
    int dummy = argc;
    dummy += 2;

    ifstream fin(argv[2]);
    ofstream fout(argv[3]);

    if (string(argv[1]) == "comprehension") {
        string line = "";
        //gets rid of first date
        while (fin >> line) {
            line.clear();
            getline(fin, line);

            //get rid of second date
            line = line.substr(10, line.size());

            double comp_score = 0;

            bool notification_participant = false;
            bool first_comma = true;
            //parse line
            for (size_t i = 0; i < line.size(); ++i) {
                vector<string> answers;
                if (line[i] == ',') {
                    //missing question 16
                    if (first_comma) {
                        notification_participant = true;
                        first_comma = false;
                    }
                    continue;
                }
                else if (line[i] == '"') {
                    string input = "";
                    for (size_t j = i + 1; j < line.size(); ++j) {
                        if (line[j] == ',') {
                            answers.push_back(input);
                            input.clear();
                            continue;
                        }
                        if (line[j] == '"') {
                            answers.push_back(input);
                            i = j + 1;
                            break;
                        }
                        input += line[j];
                    }
                }
                else {
                    string input = "";
                    for (size_t j = i; j < line.size(); ++j) {
                        if (line[j] == ',') {
                            answers.push_back(input);
                            i = j;
                            break;
                        }
                        input += line[j];
                    }
                }

                //checking answers to see if any match
                //since answers are exact and unique, only one can match
                for (size_t k = 0; k < answers.size(); ++k) {
                    //check answers for question 16
                    if (answers[k] == "Name" || answers[k] == "Social security number" || answers[k] == "Credit card number") {
                        comp_score += 0.2;
                    }
                    else if (answers[k] == "Clone your card" || answers[k] == "Use your card to make fraudulent purchases") {
                        comp_score += 0.25;
                    }
                    else if (answers[k] == "Send you unwanted advertising") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "Create new bank and credit card accounts" || answers[k] == "Commit a crime and plead guilty in your name" || answers[k] == "Use your health insurance" || answers[k] == "File fake tax returns") {
                        comp_score += (1.0 / 6);
                    }
                    else if (answers[k] == "12 months of free credit monitoring" || answers[k] == "Expert help with managing identity theft recovery" || answers[k] == "Insurance reimbursement in case of identity theft") {
                        comp_score += (1.0 / 5);
                    }
                    else if (answers[k] == "Until it is lifted") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "3 minutes") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "By placing it it at all three of the national bureaus") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "I am given a pin code that will let me thaw the freeze for a set amount of time") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "12 months") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "Credit freeze") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "Once a year") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "Incorrect SSN or home address" || answers[k] == "Unfamiliar accounts" || answers[k] == "Inquiries from creditors") {
                        comp_score += (1.0 / 5);
                    }
                    else if (answers[k] == "By going to AnnualCreditReport.com") {
                        comp_score += 1;
                    }
                    else if (answers[k] == "On https://identitytheft.gov/") {
                        comp_score += 1;
                    }
                }
            }
            if (notification_participant) {
                fout << "Missing Q16 Participant comprehension score = " + to_string(comp_score) + "\n";
            }
            else {
                fout << "Participant comprehension score = " + to_string(comp_score) + "\n";
            }
            comp_score = 0;
        }
    }
    else if (string(argv[1]) == "matrix") {
        string line = "";
        //gets rid of first date
        while (fin >> line) {
            line.clear();
            getline(fin, line);

            //get rid of second date
            line = line.substr(10, line.size());

            int trust_score = 0;
            int concern_score = 0;
            double behaviorial_score = 0;

            bool first_comma = true;
            bool missing_q32 = false;

            bool parsed_feelings = false;
            //parse line
            for (size_t i = 0; i < line.size(); ++i) {
                //check if q32 is missing
                if (line[i] == ',') {
                    if (first_comma) {
                        missing_q32 = true;
                    }
                    continue;
                }
                first_comma = false;

                if (!parsed_feelings) {
                    vector<string> answers;
                    string sub_line = "";
                    sub_line += line[i];
                    for (size_t j = i + 1; j < line.size(); ++j) {
                        if (line[j - 1] == ',' && line[j] == ',') {
                            i = j;
                            break;
                        }
                        if (line[j] == ',') {
                            answers.push_back(sub_line);
                            sub_line = "";
                            ++j;
                        }
                        sub_line += line[j];
                    }

                    //calculate answers
                    for (size_t i = 0; i < answers.size(); ++i) {
                        //answers 4 and 5 are trust
                        if (i == 4 || i == 5) {
                            trust_score += get_feeling_val(answers[i]);
                        }
                        else {
                            concern_score += get_feeling_val(answers[i]);
                        }
                    }
                    parsed_feelings = true;
                }
                else {
                    //lets parse behaviorials
                    vector<string> answers;
                    string sub_line = "";
                    for (size_t j = i; j < line.size(); ++j) {
                        if (line[j] == ',') {
                            answers.push_back(sub_line);
                            sub_line = "";
                            ++j;
                        }
                        sub_line += line[j];
                    }
                    for (size_t k = 0; k < answers.size(); ++k) {
                        behaviorial_score += get_behavior_val(answers[k]);
                    }
                    behaviorial_score = behaviorial_score / 28.0;
                    break;
                }
            }
            if (missing_q32) {
                fout << "Missing Q32 Participant concern score = " + to_string(concern_score) + ", trust score = " 
                    + to_string(trust_score) + ", behaviorial score = " + to_string(behaviorial_score) + "\n";
            }
            else {
                fout << "Participant concern score = " + to_string(concern_score) + ", trust score = "
                    + to_string(trust_score) + ", behaviorial score = " + to_string(behaviorial_score) + "\n";
            }
        }
    }
    return 0;
}