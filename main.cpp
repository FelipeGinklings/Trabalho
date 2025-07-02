#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "arvore_sbb.h"

using namespace std;

// Função para verificar e limpar notação científica
string cleanScientificNotation(const string& expr) {
    string cleaned = expr;
    // Se encontrar 'e' ou 'E', tentar substituir por formato decimal
    size_t ePos = cleaned.find_first_of("eE");
    while (ePos != string::npos) {
        // Encontrar o início do número
        size_t start = ePos;
        while (start > 0 && (isdigit(cleaned[start - 1]) || cleaned[start - 1] == '.')) {
            start--;
        }

        // Encontrar o fim do número
        size_t end = ePos + 1;
        if (end < cleaned.length() && (cleaned[end] == '+' || cleaned[end] == '-')) {
            end++;
        }
        while (end < cleaned.length() && isdigit(cleaned[end])) {
            end++;
        }

        // Extrair o número em notação científica
        string scientificNum = cleaned.substr(start, end - start);

        try {
            // Converter para double e depois para string normal
            double value = stod(scientificNum);
            ostringstream oss;
            oss << fixed << setprecision(2) << value;
            string normalNum = oss.str();

            // Substituir na string original
            cleaned.replace(start, end - start, normalNum);

            // Procurar próxima ocorrência
            ePos = cleaned.find_first_of("eE", start + normalNum.length());
        } catch (...) {
            // Se falhar, apenas procurar próxima ocorrência
            ePos = cleaned.find_first_of("eE", end);
        }
    }
    return cleaned;
}

void processExpression(const string& expression, int size, const string& description) {
    cout << "\n" << string(60, '=') << endl;
    cout << description << " (Tamanho: " << size << ")" << endl;
    cout << string(60, '=') << endl;

    // Limpar qualquer notação científica
    string cleanedExpression = cleanScientificNotation(expression);
    cout << "Expressão: " << cleanedExpression << endl;

    try {
        ParenthesisData* parenthesis_data = separate_by_parenthesis(cleanedExpression);
        if (parenthesis_data == nullptr) {
            cout << "ERRO: Falha ao processar parênteses" << endl;
            return;
        }

        create_sub_trees(parenthesis_data);
        create_tree(parenthesis_data);

        cout << "Árvore em ordem: ";
        in_order(parenthesis_data->tree);
        cout << endl;

        // Mostrar árvore visual apenas para tamanho 10
        if (size == 10) {
            cout << "\nEstrutura da árvore:" << endl;
            print_tree(parenthesis_data->tree);
        }

        const double result = calculate_tree(parenthesis_data->tree);
        cout << "Resultado: " << fixed << setprecision(2) << result << endl;

        // Limpar memória
        parenthesis_data->deleteAll();
        delete parenthesis_data;

    } catch (const exception& e) {
        cout << "ERRO ao processar expressão: " << e.what() << endl;
    }
}

int main() {
    cout << "DEMONSTRAÇÃO DE CÁLCULO DE ÁRVORES DE EXPRESSÕES" << endl;
    cout << "================================================" << endl;

    // Caso 1: Expressões sem parênteses e sem números decimais (apenas inteiros)
    cout << "\n\n### CASO 1: EXPRESSÕES COM NÚMEROS INTEIROS (SEM PARÊNTESES) ###" << endl;
    for (int size = 1; size <= 10; size++) {
        string expression = generateExpression(size, false, false, 1, 200);
        processExpression(expression, size, "Expressão com inteiros");
    }

    // Caso 2: Expressões sem parênteses com números decimais
    cout << "\n\n### CASO 2: EXPRESSÕES COM NÚMEROS DECIMAIS (SEM PARÊNTESES) ###" << endl;
    for (int size = 1; size <= 10; size++) {
        string expression;
        int attempts = 0;
        do {
            expression = generateExpression(size, false, true, 1, 99, 1.10, 99.90);
            expression = cleanScientificNotation(expression);
            attempts++;
        } while ((expression.find('e') != string::npos || expression.find('E') != string::npos) && attempts < 5);

        // Se ainda tiver notação científica após 5 tentativas, usar uma expressão manual
        if (expression.find('e') != string::npos || expression.find('E') != string::npos) {
            vector<string> manual_decimals = {"12.50", "10.30+5.20", "15.10*2.50+4.30", "20.40/2.10+3.60*7.80"};
            if (size <= manual_decimals.size()) {
                expression = manual_decimals[size - 1];
            } else {
                expression = "10.50+5.25";  // fallback
            }
        }

        processExpression(expression, size, "Expressão com números decimais");
    }

    // Caso 3: Expressões simples com parênteses (casos controlados)
    cout << "\n\n### CASO 3: EXPRESSÕES COM PARÊNTESES (CASOS SIMPLES) ###" << endl;

    // Casos controlados manualmente para evitar problemas de geração
    vector<string> expressions_with_parentheses = {"42",          "10+5",           "(15+3)*2",      "(20-5)/3+1",    "10*(8+7)", "(15+5)/(10-6)",
                                                   "(3+4)*(5-2)", "(12+8)/(4-2)*3", "(6+4)*(3+2)/5", "(10+5)*(8-3)/2"};

    for (int i = 0; i < expressions_with_parentheses.size(); i++) {
        processExpression(expressions_with_parentheses[i], i + 1, "Expressão com parênteses");
    }

    // Caso 4: Mix de inteiros e decimais (sem parênteses para estabilidade)
    cout << "\n\n### CASO 4: EXPRESSÕES MISTAS INTEIROS + DECIMAIS ###" << endl;
    vector<string> mixed_expressions = {"12.50",
                                        "10+5.50",
                                        "15.20*3+4",
                                        "20.50/2.50+3*7",
                                        "12.30+15-4.70*2.10",
                                        "25.50/5+18*2.20-6.80",
                                        "100.50-25.30*2+15/3.50+8",
                                        "45.20*1.50+30/6-12.80+7.30*2",
                                        "75.50/2.50*3+40-18.20/3.10+25*1.80",
                                        "120.50-35.70*2.20+60/4.80-25+18.90*3.30"};

    for (int i = 0; i < mixed_expressions.size(); i++) {
        processExpression(mixed_expressions[i], i + 1, "Expressão mista");
    }

    // Casos especiais
    cout << "\n\n### CASOS ESPECIAIS ###" << endl;

    processExpression("100", 1, "Número único");
    processExpression("50+25", 2, "Soma simples");
    processExpression("12*8/4", 3, "Multiplicação e divisão");
    processExpression("(25+15)/2", 3, "Parênteses com divisão");
    processExpression("15.50+7.30", 2, "Soma de decimais");
    processExpression("(12.50+7.50)/4", 3, "Parênteses com decimais");

    cout << "\n" << string(60, '=') << endl;
    cout << "DEMONSTRAÇÃO CONCLUÍDA" << endl;
    cout << string(60, '=') << endl;

    return 0;
}
