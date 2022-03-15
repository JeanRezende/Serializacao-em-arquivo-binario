#ifndef MBTREE_H_INCLUDED
#define MBTREE_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <type_traits>

using namespace std;

#include "node.h"
//#include "record.h"
#include "intserial.h"

const ios::openmode mode = ios::in | ios::out | ios::binary;


template <class T, const unsigned int MIN_DEGREE>
class mbtree : private fstream {
   public:
      mbtree();
      mbtree(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      virtual ~mbtree();
      bool open(const string name, const string type, const unsigned int version, ios::openmode openmode = mode);
      bool isOpen();
      bool close();
      void clear();
      bool readRecord(T k, unsigned long long int i);
      bool writeRecord(T k, unsigned long long int i);
      bool insertRecord(T k);
      bool removeRecord(T k);
      int search(node<T, MIN_DEGREE>* x, T k);
      void print();
      unsigned long long int getFirstValid();
      unsigned long long int getFirstDeleted();
   protected:
        header head;
        bool readHeader(header &h);
      bool writeHeader(header &h);
      unsigned long long int index2pos(unsigned long long int i);
      unsigned long long int pos2index(unsigned long long int p);
      void insertNonFull(node<T, MIN_DEGREE>* x, T k);
      void removeAux(node<T, MIN_DEGREE>* x, T k);
      T findPredecessor(node<T, MIN_DEGREE>* x);
      T findSuccessor(node<T, MIN_DEGREE>* x);
      void printAux(node<T, MIN_DEGREE>* x, vector<string> &v, unsigned int lvl);
      int searchAux(node<T, MIN_DEGREE>* x, T k);
      node<T, MIN_DEGREE>* root = NULL;
};
template <class T, const unsigned int MIN_DEGREE>
mbtree<T, MIN_DEGREE>::mbtree() : fstream() {
//   if (!root)
//      root = new node<T, MIN_DEGREE>();
}

template <class T, const unsigned int MIN_DEGREE>
mbtree<T, MIN_DEGREE>::mbtree(const string name, const string type,
                               const unsigned int version, ios::openmode openmode) : fstream(name.c_str(), mode) {
        if (!isOpen()){
        fstream::open(name.c_str(), mode | ios::trunc);
        if (isOpen()){
            cout << "--- New file dat created! ---" << endl;
            header aux(type, version);
            this->head = aux;
            this->head.setFirstValid(1);
            if(writeHeader(aux)){
            cout << "--- Header wrote on file! ---" << endl;
        }
        else
            cout << "--- Error creating the file! ---" << endl;
        }
    }else{
        cout << "--- File found and opened! ---" << endl;
        if(readHeader(head) && head.getType() == type && head.getVersion() == version){

            cout << "--- Compatible headers, access allowed! ---" << endl;
            if(readHeader(head)){
                header aux(type, version);
                aux.setFirstValid(head.getFirstValid());
                aux.setFirstDeleted(head.getFirstDeleted());
                this->head = aux;
            }
        }else{
            cout << "--- Incompatible headers, access denied! ---" << endl;
            this->close();
        }
     }
}

template <class T, const unsigned int MIN_DEGREE>
mbtree<T, MIN_DEGREE>::~mbtree() {
   delete root;
   root = NULL;
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::open(const string name, const string type, const unsigned int ver, ios::openmode openmode) {
    if(!isOpen()){
        fstream::open(name, openmode);
        if(isOpen()){
            cout << "arquivo criado " << endl;
        }
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::isOpen() {
    return fstream::is_open();
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::close() {
    fstream::close();
    return !isOpen();//se tiver fechato retorna falso
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::clear() {

}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::readRecord(T k, unsigned long long int i) {
    if(isOpen())
    {
        fstream::seekg(i);
        char *aux = new char[r.size()];
        fstream::read(aux, r.size());
        r.fromString(string(aux,r.size()));
        delete aux;
        return fstream::good();
    }
    else{
        cout << " erro de leitura " << endl;
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::writeRecord(T k, unsigned long long int i) {
    if(isOpen()){
        fstream::seekp(pos2index(i), ios::beg);
        fstream::write(r.toString().c_str, r.size());
        return fstream::good();
    }
    else {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::insertRecord(T k) {
   
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::removeRecord(T k) {
    //this->head.getFirstDeleted
}

/*
template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::insert(T k) {
   node<T, MIN_DEGREE> *r = root, *s = NULL;

   if (r->size() == node<T, MIN_DEGREE>::MAX) {
      s = new node<T, MIN_DEGREE>();
      root = s;

      s->insertChild(r);
      s->splitChild(0);
      insertNonFull(s, k);
   } else {
      insertNonFull(r, k);
   }
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::remove(T k) {
   node<T, MIN_DEGREE>* x = root;

   removeAux(x, k);
}
*/
template <class T, const unsigned int MIN_DEGREE>
int mbtree<T, MIN_DEGREE>::search(node<T, MIN_DEGREE>* x, T k) {
   x = root;
   return searchAux(x, k);
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::print() {
   int lvl = 0;
   vector<string> levels(1);
   printAux(root, levels, lvl);

   for (string s : levels) {
      cout << s << endl;
   }
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::insertNonFull(node<T, MIN_DEGREE>* x, T k) {
   int i = x->size() - 1;
   node<T, MIN_DEGREE>* y = NULL;

   if (x->isLeaf()) {
      x->insertKey(k);
   } else {
      while (i >= 0 && k < x->getKey(i)) {
         i--;
      }

      i++;
      y = x->getChild(i);

      if (y->size() == node<T, MIN_DEGREE>::MAX) {
         x->splitChild(i);

         if (k > x->getKey(i)) {
            i++;
         }
      }

      insertNonFull(x->getChild(i), k);
   }
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::removeAux(node<T, MIN_DEGREE>* x, T k) {
   unsigned int pos = x->hasKey(k);
   T aux;

   if (x->isLeaf()) { // Case 1
      if (pos != node<T, MIN_DEGREE>::NOT_FOUND) {
         x->removeKey(pos);
      }
   } else {
      if (pos != node<T, MIN_DEGREE>::NOT_FOUND && !x->isLeaf()) { // Case 2
         if (x->getChild(pos)->size() > node<T, MIN_DEGREE>::MIN) { // Case 2a
            aux = findPredecessor(x->getChild(pos));
            removeAux(x->getChild(pos), aux);
            x->setKey(aux, pos);
         } else {
            if (x->getChild(pos + 1)->size() > node<T, MIN_DEGREE>::MIN) { // Case 2b
               aux = findSuccessor(x->getChild(pos + 1));
               removeAux(x->getChild(pos + 1), aux);
               x->setKey(aux, pos);
            } else { // 2c
               x->mergeChildren(pos);
               if (x == root && x->size() == 0) {
                  root = x->getChild(pos);
                  delete x;
                  removeAux(root, k);
               } else {
                  removeAux(x->getChild(pos), k);
               }
            }
         }
      } else { // Case 3
         pos = 0;

         while (pos <= x->size() && k > x->getKey(pos)) {
            pos++;
         }

         if (x->getChild(pos)->size() == node<T, MIN_DEGREE>::MIN) { // Case 3a
            if (pos > 0 && x->getChild(pos - 1)->size() > node<T, MIN_DEGREE>::MIN) {
               x->rotateKeys(pos - 1);
            } else {
               if (pos < x->size() && x->getChild(pos + 1)->size() > node<T, MIN_DEGREE>::MIN) {
                  x->rotateKeys(pos);
               } else { // Case 3b
                  if (pos == x->size()) {
                     x->mergeChildren(pos - 1);
                  } else {
                     x->mergeChildren(pos);
                  }

                  if (x == root && x->size() == 0) {
                     root = x->getChild(0);
                     delete x;
                     x = root;
                     pos = 0;

                     while (pos <= x->size() && k > x->getKey(pos)) {
                        pos++;
                     }
                  }
               }
            }
         }

         removeAux(x->getChild(pos), k);
      }
   }
}

template <class T, const unsigned int MIN_DEGREE>
T mbtree<T, MIN_DEGREE>::findPredecessor(node<T, MIN_DEGREE>* x) {
   if (x->isLeaf()) {
      return x->getKey(x->size() - 1);
   } else {
      return findPredecessor(x->getChild(x->size()));
   }
}

template <class T, const unsigned int MIN_DEGREE>
T mbtree<T, MIN_DEGREE>::findSuccessor(node<T, MIN_DEGREE>* x) {
   if (x->isLeaf()) {
      return x->getKey(0);
   } else {
      return findSuccessor(x->getChild(0));
   }
}

template <class T, const unsigned int MIN_DEGREE>
void mbtree<T, MIN_DEGREE>::printAux(node<T, MIN_DEGREE>* x, vector<string> &v, unsigned int lvl) {
   string aux = "[";
   unsigned int i = 0;

   if (v.size() < lvl + 1) {
      v.resize(lvl + 1);
   }

   if (!x->isLeaf()) {
      for (i = 0; i <= x->size(); i++) {
         if (x->getChild(i) != NULL)
            printAux(x->getChild(i), v, lvl + 1);
      }
   }

   for (i = 0; i < x->size(); i++) {
      aux += to_string(x->getKey(i)) + ", ";
   }

   if (aux.length() > 1) {
      aux += "\b\b] ";
   } else {
      aux += "] ";
   }

   v[lvl] += aux;
}

template <class T, const unsigned int MIN_DEGREE>
int mbtree<T, MIN_DEGREE>::searchAux(node<T, MIN_DEGREE>* x, T k) {
   unsigned int i = 0;

   while (i < x->size() && k > x->getKey(i)) {
      i++;
   }

   if (i < x->size() && k == x->getKey(i)) {
      return i;
   } else {
      if (x->isLeaf()) {
         x = NULL;
         return -1;
      } else {
         return searchAux(x->getChild(i), k);
      }
   }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int mbtree<T, MIN_DEGREE>::getFirstValid() {
    return this->head.getFirstValid();
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int mbtree<T, MIN_DEGREE>::getFirstDeleted() {
    return this->head.getFirstDeleted();
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::readHeader(header &h) {
    if (isOpen())
    {
        char *aux = new char[h.size()];
        fstream::seekg(0, ios::beg);
        fstream::read(aux, h.size());
        h.fromString(string(aux, h.size()));
        delete aux;
        return fstream::good();
    }
    else
    {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
bool mbtree<T, MIN_DEGREE>::writeHeader(header &h) {
    if (isOpen())
    {
        fstream::seekp(0, ios::beg);
        fstream::write(h.toString().c_str(), h.size());
        return fstream::good();
    }
    else
    {
        return false;
    }
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int mbtree<T, MIN_DEGREE>::index2pos(unsigned long long int i) {
    //<intSerial> aux;
    //return ( i - head.size() / aux.size())
}

template <class T, const unsigned int MIN_DEGREE>
unsigned long long int mbtree<T, MIN_DEGREE>::pos2index(unsigned long long int p) {
    //TODO
    return head.size() + p ;
}


#endif // MBTREE_H_INCLUDED
