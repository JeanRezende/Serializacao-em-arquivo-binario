#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include <iostream>
#include <vector>
#include "serializable.h"
#include "intserial.h"

using namespace std;


template <class T, const unsigned int MIN_DEGREE>
class node : public serializable{
   public:
      node();
      virtual ~node();
      bool isLeaf();
      unsigned int size();
      void resize(unsigned int s);
      void setKey(T k, unsigned int i);
      void insertKey(T k);
      unsigned int hasKey(T k);
      void removeKey(unsigned int i);
      T getKey(int i);
      void setChild(node<T, MIN_DEGREE>* c, unsigned int i);
      void insertChild(node<T, MIN_DEGREE>* c);
      node<T, MIN_DEGREE>* getChild(unsigned int i);
      void splitChild(unsigned int i);
      void mergeChildren(unsigned int i);
      void rotateKeys(unsigned int i);
      node<T, MIN_DEGREE> operator=(const node<T, MIN_DEGREE> &other);
      virtual string toCSV() { return ""; }
      virtual void fromCSV(string repr) {}
      virtual string toJSON() { return ""; }
      virtual void fromJSON(string repr) {}
      virtual string toXML() { return ""; }
      virtual void fromXML(string repr) {}
      virtual string toString();
      virtual void fromString(string repr);
      virtual unsigned long long int size() const;
      static const unsigned int MAX = 2 * MIN_DEGREE - 1;
      static const unsigned int MIN = MIN_DEGREE - 1;
      static const unsigned int NOT_FOUND = -1;
   protected:
      vector<T> keys;
      vector<node<T, MIN_DEGREE>*> children; //pointers to children
      bool leaf;
      unsigned int n; //elements
};

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::node() : serializable(){
   unsigned int i;
   leaf = true;
   n = 0;
   keys.resize(MAX);
   children.resize(MAX + 1);
   for (i = 0; i < children.size(); i++) {
      children[i] = nullptr;
   }
}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>::~node() {
   unsigned int i;
   for (i = 0; i < n; i++) {
      if (children[i]) {
         delete children[i];
         children[i] = nullptr;
      }
   }
}

template <class T, const unsigned int MIN_DEGREE>
bool node<T, MIN_DEGREE>::isLeaf() {
   return leaf;
}

template <class T, const unsigned int MIN_DEGREE>
unsigned int node<T, MIN_DEGREE>::size() {
   return n;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::resize(unsigned int s) {
   unsigned int i;

   for (i = s + 1; i <= n; i++){
      children[i] = NULL;
   }

   if (s == 0) {
      children[0] = NULL;
   }

   n = s;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::setKey(T k, unsigned int i) {
   keys[i] = k;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::insertKey(T k) {
   int i = n - 1;

   while (i >= 0 && k < keys[i]) {
      keys[i + 1] = keys[i];
      i--;
   }

   keys[i + 1] = k;
   resize(n + 1);
}

template <class T, const unsigned int MIN_DEGREE>
unsigned int node<T, MIN_DEGREE>::hasKey(T k) {
   unsigned int i = 0;
   int pos = NOT_FOUND;

   while (i < n && pos == -1) {
      if (keys[i] == k) {
         pos = i;
      }
      i++;
   }

   return pos;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::removeKey(unsigned int i) {
   unsigned int j = i;

   while (j < n - 1) {
      keys[j] = keys[j + 1];
      j++;
   }

   if (!leaf) {
      j = i + 1;
      while (j < n) {
         children[j] = children[j + 1];
         j++;
      }
   }
   resize(n - 1);
}

template <class T, const unsigned int MIN_DEGREE>
T node<T, MIN_DEGREE>::getKey(int i) {
   return keys[i];
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::setChild(node<T, MIN_DEGREE>* c, unsigned int i) {
   if (c) {
      children[i] = c;
      leaf = false;
   }
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::insertChild(node<T, MIN_DEGREE>* c) {
   int i = n;

   while (i >= 0 && keys[i - 1] > c->getKey(0)) {
      children[i + 1] = children[i];
      i--;
   }

   children[i] = c;
   leaf = false;
}

template <class T, const unsigned int MIN_DEGREE>
node<T, MIN_DEGREE>* node<T, MIN_DEGREE>::getChild(unsigned int i) {
   return children[i];
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::splitChild(unsigned int i) {
   node<T, MIN_DEGREE> *y = getChild(i);
   node<T, MIN_DEGREE> *z = new node<T, MIN_DEGREE>();
   T middle = y->getKey(MIN_DEGREE - 1);
   unsigned int j = 0;

   for (j = 0; j < MIN_DEGREE - 1; j++) {
      z->insertKey(y->getKey(j + MIN_DEGREE));
   }

   if (!y->isLeaf()) {
      for (j = 0; j < MIN_DEGREE; j++) {
         z->insertChild(y->getChild(j + MIN_DEGREE));
      }
   }

   for (j = n; j > i; j--) {
      children[j + 1] = children[j];
   }

   children[j + 1] = z;

   insertKey(middle);

   y->resize(MIN);
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::mergeChildren(unsigned int i) {
   node<T, MIN_DEGREE>* y = getChild(i);
   node<T, MIN_DEGREE>* z = getChild(i + 1);
   unsigned int j;

   y->insertKey(keys[i]);

   for (j = 0; j < MIN; j++) {
      y->insertKey(z->getKey(j));
   }

   if (!y->isLeaf()) {
      for (j = 0; j < MIN + 1; j++) {
         y->setChild(z->getChild(j), j + MIN + 1);
      }
   }

   removeKey(i);

   children[i] = y;

   z->resize(0);
   delete z;
   z = NULL;
}

template <class T, const unsigned int MIN_DEGREE>
void node<T, MIN_DEGREE>::rotateKeys(unsigned int i) {
   node<T, MIN_DEGREE>* y = getChild(i);
   node<T, MIN_DEGREE>* z = getChild(i + 1);

   if (y->size() > MIN) {
      z->insertKey(keys[i]);
      keys[i] = y->getKey(y->size() - 1);

      if (!y->isLeaf()) {
         z->insertChild(y->getChild(y->size()));
      }

      y->resize(y->size() - 1);
   } else {
      y->insertKey(keys[i]);
      keys[i] = z->getKey(0);

      if (!z->isLeaf()) {
         y->insertChild(z->getChild(0));
      }

      z->removeKey(0);
   }
}

#endif // NODE_H_INCLUDED
