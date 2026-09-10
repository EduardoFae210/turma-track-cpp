#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>

using namespace std;

const int QUANTIDADE_AVALIACOES = 3;
const double MEDIA_APROVACAO = 7.0;

struct Aluno {
    int codigo;
    string nome;
};

void exibirTitulo() {
    cout << "\n============================================\n";
    cout << "                 TURMATRACK                 \n";
    cout << "       Acompanhamento de desempenho         \n";
    cout << "============================================\n";
}

void limparEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lerInteiro(const string& mensagem, int minimo, int maximo) {
    int valor;

    while (true) {
        cout << mensagem;
        cin >> valor;

        if (!cin.fail() && valor >= minimo && valor <= maximo) {
            limparEntrada();
            return valor;
        }

        cout << "Entrada invalida. Digite um numero entre "
             << minimo << " e " << maximo << ".\n";
        limparEntrada();
    }
}

double lerNota(const string& mensagem) {
    double nota;

    while (true) {
        cout << mensagem;
        cin >> nota;

        if (!cin.fail() && nota >= 0.0 && nota <= 10.0) {
            limparEntrada();
            return nota;
        }

        cout << "Nota invalida. Digite um valor entre 0 e 10.\n";
        limparEntrada();
    }
}

string lerNome() {
    string nome;

    while (nome.empty()) {
        cout << "Nome do aluno: ";
        getline(cin, nome);

        if (nome.empty()) {
            cout << "O nome nao pode ficar vazio.\n";
        }
    }

    return nome;
}

int buscarIndicePorCodigo(const vector<Aluno>& alunos, int codigo) {
    for (int i = 0; i < static_cast<int>(alunos.size()); i++) {
        if (alunos[i].codigo == codigo) {
            return i;
        }
    }

    return -1;
}

bool possuiTodasAsNotas(const vector<vector<double>>& matrizNotas, int indice) {
    for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
        if (matrizNotas[indice][avaliacao] < 0.0) {
            return false;
        }
    }

    return true;
}

double calcularMedia(const vector<vector<double>>& matrizNotas, int indice) {
    if (!possuiTodasAsNotas(matrizNotas, indice)) {
        return -1.0;
    }

    double soma = 0.0;

    for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
        soma += matrizNotas[indice][avaliacao];
    }

    return soma / QUANTIDADE_AVALIACOES;
}

string definirSituacao(double media) {
    if (media < 0.0) {
        return "Pendente";
    }

    if (media >= MEDIA_APROVACAO) {
        return "Aprovado";
    }

    return "Reprovado";
}

void cadastrarAluno(vector<Aluno>& alunos, vector<vector<double>>& matrizNotas) {
    cout << "\n--- Cadastro de aluno ---\n";
    int codigo = lerInteiro("Codigo do aluno: ", 1, 999999);

    if (buscarIndicePorCodigo(alunos, codigo) != -1) {
        cout << "Ja existe um aluno com esse codigo.\n";
        return;
    }

    Aluno novoAluno;
    novoAluno.codigo = codigo;
    novoAluno.nome = lerNome();

    alunos.push_back(novoAluno);

    vector<double> notasIniciais;
    for (int i = 0; i < QUANTIDADE_AVALIACOES; i++) {
        notasIniciais.push_back(-1.0);
    }
    matrizNotas.push_back(notasIniciais);

    cout << "Aluno cadastrado com sucesso.\n";
}

void registrarNotas(const vector<Aluno>& alunos, vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    cout << "\n--- Registro de notas ---\n";
    int codigo = lerInteiro("Codigo do aluno: ", 1, 999999);
    int indice = buscarIndicePorCodigo(alunos, codigo);

    if (indice == -1) {
        cout << "Aluno nao encontrado.\n";
        return;
    }

    cout << "Aluno: " << alunos[indice].nome << "\n";

    for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
        string mensagem = "Nota da avaliacao " + to_string(avaliacao + 1) + ": ";
        matrizNotas[indice][avaliacao] = lerNota(mensagem);
    }

    cout << "Notas registradas com sucesso.\n";
}

void exibirCabecalhoTabela() {
    cout << left << setw(8) << "Codigo"
         << setw(24) << "Nome"
         << right << setw(8) << "N1"
         << setw(8) << "N2"
         << setw(8) << "N3"
         << setw(9) << "Media"
         << setw(12) << "Situacao" << "\n";
    cout << string(77, '-') << "\n";
}

void exibirLinhaAluno(const Aluno& aluno,
                      const vector<vector<double>>& matrizNotas,
                      int indice) {
    cout << left << setw(8) << aluno.codigo
         << setw(24) << aluno.nome << right;

    for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
        if (matrizNotas[indice][avaliacao] < 0.0) {
            cout << setw(8) << "-";
        } else {
            cout << fixed << setprecision(1)
                 << setw(8) << matrizNotas[indice][avaliacao];
        }
    }

    double media = calcularMedia(matrizNotas, indice);
    if (media < 0.0) {
        cout << setw(9) << "-";
    } else {
        cout << fixed << setprecision(1) << setw(9) << media;
    }

    cout << setw(12) << definirSituacao(media) << "\n";
}

void listarAlunos(const vector<Aluno>& alunos,
                  const vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    cout << "\n--- Alunos da turma ---\n";
    exibirCabecalhoTabela();

    for (int i = 0; i < static_cast<int>(alunos.size()); i++) {
        exibirLinhaAluno(alunos[i], matrizNotas, i);
    }
}

void buscarAluno(const vector<Aluno>& alunos,
                 const vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    cout << "\n--- Busca sequencial ---\n";
    int codigo = lerInteiro("Codigo do aluno: ", 1, 999999);
    int indice = buscarIndicePorCodigo(alunos, codigo);

    if (indice == -1) {
        cout << "Aluno nao encontrado.\n";
        return;
    }

    exibirCabecalhoTabela();
    exibirLinhaAluno(alunos[indice], matrizNotas, indice);
}

vector<int> ordenarIndicesPorMedia(const vector<Aluno>& alunos,
                                   const vector<vector<double>>& matrizNotas) {
    vector<int> indices;

    for (int i = 0; i < static_cast<int>(alunos.size()); i++) {
        indices.push_back(i);
    }

    // Bubble sort em ordem decrescente, mantendo a turma original inalterada.
    for (int passagem = 0; passagem < static_cast<int>(indices.size()) - 1; passagem++) {
        for (int atual = 0;
             atual < static_cast<int>(indices.size()) - passagem - 1;
             atual++) {
            double mediaAtual = calcularMedia(matrizNotas, indices[atual]);
            double proximaMedia = calcularMedia(matrizNotas, indices[atual + 1]);

            if (mediaAtual < proximaMedia) {
                int temporario = indices[atual];
                indices[atual] = indices[atual + 1];
                indices[atual + 1] = temporario;
            }
        }
    }

    return indices;
}

void exibirRanking(const vector<Aluno>& alunos,
                   const vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    vector<int> indicesOrdenados = ordenarIndicesPorMedia(alunos, matrizNotas);

    cout << "\n--- Ranking por media ---\n";
    cout << left << setw(10) << "Posicao"
         << setw(8) << "Codigo"
         << setw(24) << "Nome"
         << right << setw(10) << "Media" << "\n";
    cout << string(52, '-') << "\n";

    for (int posicao = 0; posicao < static_cast<int>(indicesOrdenados.size()); posicao++) {
        int indice = indicesOrdenados[posicao];
        double media = calcularMedia(matrizNotas, indice);

        cout << left << setw(10) << posicao + 1
             << setw(8) << alunos[indice].codigo
             << setw(24) << alunos[indice].nome
             << right;

        if (media < 0.0) {
            cout << setw(10) << "Pendente";
        } else {
            cout << fixed << setprecision(1) << setw(10) << media;
        }

        cout << "\n";
    }
}

void exibirMatrizDeNotas(const vector<Aluno>& alunos,
                         const vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    cout << "\n--- Matriz de notas ---\n";
    cout << left << setw(8) << "Codigo" << setw(24) << "Aluno";

    for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
        cout << right << setw(8) << ("AV" + to_string(avaliacao + 1));
    }
    cout << "\n" << string(56, '-') << "\n";

    for (int aluno = 0; aluno < static_cast<int>(alunos.size()); aluno++) {
        cout << left << setw(8) << alunos[aluno].codigo
             << setw(24) << alunos[aluno].nome << right;

        for (int avaliacao = 0; avaliacao < QUANTIDADE_AVALIACOES; avaliacao++) {
            if (matrizNotas[aluno][avaliacao] < 0.0) {
                cout << setw(8) << "-";
            } else {
                cout << fixed << setprecision(1)
                     << setw(8) << matrizNotas[aluno][avaliacao];
            }
        }
        cout << "\n";
    }
}

void exibirRelatorio(const vector<Aluno>& alunos,
                     const vector<vector<double>>& matrizNotas) {
    if (alunos.empty()) {
        cout << "\nNenhum aluno cadastrado.\n";
        return;
    }

    int concluidos = 0;
    int aprovados = 0;
    int reprovados = 0;
    int indiceMaiorMedia = -1;
    int indiceMenorMedia = -1;
    double somaDasMedias = 0.0;

    for (int i = 0; i < static_cast<int>(alunos.size()); i++) {
        double media = calcularMedia(matrizNotas, i);

        if (media >= 0.0) {
            concluidos++;
            somaDasMedias += media;

            if (media >= MEDIA_APROVACAO) {
                aprovados++;
            } else {
                reprovados++;
            }

            if (indiceMaiorMedia == -1 ||
                media > calcularMedia(matrizNotas, indiceMaiorMedia)) {
                indiceMaiorMedia = i;
            }

            if (indiceMenorMedia == -1 ||
                media < calcularMedia(matrizNotas, indiceMenorMedia)) {
                indiceMenorMedia = i;
            }
        }
    }

    cout << "\n--- Relatorio da turma ---\n";
    cout << "Alunos cadastrados: " << alunos.size() << "\n";
    cout << "Alunos com notas completas: " << concluidos << "\n";

    if (concluidos == 0) {
        cout << "Ainda nao ha notas completas para calcular o relatorio.\n";
        return;
    }

    double mediaGeral = somaDasMedias / concluidos;
    double percentualAprovados = aprovados * 100.0 / concluidos;
    double percentualReprovados = reprovados * 100.0 / concluidos;

    cout << fixed << setprecision(1);
    cout << "Media geral: " << mediaGeral << "\n";
    cout << "Aprovados: " << aprovados << " (" << percentualAprovados << "%)\n";
    cout << "Reprovados: " << reprovados << " (" << percentualReprovados << "%)\n";
    cout << "Maior media: " << alunos[indiceMaiorMedia].nome
         << " (" << calcularMedia(matrizNotas, indiceMaiorMedia) << ")\n";
    cout << "Menor media: " << alunos[indiceMenorMedia].nome
         << " (" << calcularMedia(matrizNotas, indiceMenorMedia) << ")\n";
}

void exibirMenu() {
    cout << "\n[1] Cadastrar aluno\n";
    cout << "[2] Registrar notas\n";
    cout << "[3] Listar alunos\n";
    cout << "[4] Buscar aluno\n";
    cout << "[5] Exibir ranking\n";
    cout << "[6] Exibir matriz de notas\n";
    cout << "[7] Exibir relatorio da turma\n";
    cout << "[0] Encerrar\n";
}

int main() {
    vector<Aluno> alunos;
    vector<vector<double>> matrizNotas;
    int opcao;

    exibirTitulo();

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0, 7);

        if (opcao == 1) {
            cadastrarAluno(alunos, matrizNotas);
        } else if (opcao == 2) {
            registrarNotas(alunos, matrizNotas);
        } else if (opcao == 3) {
            listarAlunos(alunos, matrizNotas);
        } else if (opcao == 4) {
            buscarAluno(alunos, matrizNotas);
        } else if (opcao == 5) {
            exibirRanking(alunos, matrizNotas);
        } else if (opcao == 6) {
            exibirMatrizDeNotas(alunos, matrizNotas);
        } else if (opcao == 7) {
            exibirRelatorio(alunos, matrizNotas);
        }
    } while (opcao != 0);

    cout << "\nPrograma encerrado. Ate a proxima!\n";
    return 0;
}
