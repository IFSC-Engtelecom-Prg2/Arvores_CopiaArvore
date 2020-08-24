//
// Created by sobral on 27/07/19.
//
#include <limits.h>
#include <time.h>
#include <string>
#include <algorithm>
#include <vector>
#include <ostream>
#include <sstream>
#include <fstream>
#include <list>
#include <libs/arvore.h>
#include <chrono>
#include <random>
#include "questao.h"
#include "gtest/gtest.h"

using namespace std;
using prglib::arvore;

class TesteQuestao: public ::testing::Test {
protected:
    virtual void SetUp() {
        srand(clock());
    }

    template <typename T> void escrevaSe(ostream & out, list<T> & l, const string & sep) {
        if (l.empty()) return;
        auto it = l.begin();
        out << *it;
        for (; it != l.end(); it++) out << sep << *it;
    }

    template <class T> void embaralha(list<T> & l) {
        if (l.size() < 2) return;
        static bool _started = false;
        static std::default_random_engine generator;

        if (not _started) {
            auto t = std::chrono::high_resolution_clock::now();
            _started = true;
            generator.seed(std::chrono::high_resolution_clock::to_time_t(t));
        }
        vector<T> v(l.begin(), l.end());
        shuffle(v.begin(), v.end(), generator);
        l.clear();
        l.insert(l.begin(),v.begin(), v.end());
    }

    bool compara(arvore<string> * a1, arvore<string> * a2) {
        // implemente aqui sua função ...
        list<string> l1, l2;

        if (a1) a1->listePreOrder(l1);
        if (a2) a2->listePreOrder(l2);

        return l1 == l2;
    }
};

TEST_F(TesteQuestao, Nulo){
    arvore<long> * arv;

    try {
        arv = copiaArvore<long>(nullptr);
    } catch (...) {
        FAIL() << "copiaArvore disparou uma excecao ... ";
        return;
    }

    if (arv != nullptr) {
        FAIL() << "copiaArvore retornou um ponteiro diferente de nullptr, quando deveria retornar nullptr";
        return;
    }
}

TEST_F(TesteQuestao, Normal)
{
    arvore<long> * orig;
    arvore<long> * arv;

    srand(time(NULL));

    orig = new arvore<long>(rand() % 100000);
    for (int n=0; n < 80; n++) orig->adiciona(rand() % 100000);
    orig = orig->balanceia(true);
    list<long> l;
    orig->listePreOrder(l);

    try {
        arv = copiaArvore<long>(orig);
    } catch (...) {
        FAIL() << "copiaArvore disparou uma excecao ... ";
        return;
    }

    if (arv == nullptr) {
        FAIL() << "copiaArvore retornou nullptr, quando deveria retornar um ponteiro para nova arvore";
        return;
    }

    list<long> l2;
    arv->listePreOrder(l2);

    if (not (l == l2)) {
        string r = "Conteúdo esperado: ";
        ostringstream s1, s2;
        escrevaSe(s1, l, "\n");
        r += s1.str();
        r+="\n\nConteúdo encontrado: ";
        escrevaSe(s2, l2, "\n");
        r += s2.str();

        FAIL() << "Nova arvore diferente da original:\n\n" + r;
        return;
    }
}

TEST_F(TesteQuestao, Desbalanceada)
{
    arvore<long> * orig;
    arvore<long> * arv;

    srand(time(NULL));

    list<long> l;
    for (int n=0; n < 80; n++) l.push_back(rand() % 100000);
    l.sort();
    auto it = l.begin();
    orig = new arvore<long>(*it);
    for (; it != l.end(); it++) {
        orig->adiciona(*it);
    }
    l.clear();
    orig->listePreOrder(l);

    try {
        arv = copiaArvore<long>(orig);
    } catch (...) {
        FAIL() << "copiaArvore disparou uma excecao ... ";
        return;
    }

    if (arv == nullptr) {
        FAIL() << "copiaArvore retornou nullptr, quando deveria retornar um ponteiro para nova arvore";
        return;
    }

    list<long> l2;
    arv->listePreOrder(l2);

    if (not (l == l2)) {
        string r = "Conteúdo esperado: ";
        ostringstream s1, s2;
        escrevaSe(s1, l, "\n");
        r += s1.str();
        r+="\n\nConteúdo encontrado: ";
        escrevaSe(s1, l2, "\n");
        r += s2.str();

        FAIL() << "Nova arvore diferente da original:\n\n" + r;
        return;
    }
}