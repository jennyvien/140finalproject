#include <iostream>
#include <iomanip>

using namespace std;

class Bag{
	public:
		int *elementsArray; // the vertices in the array
		int arraySize; // the length array
		int index; // the number of elements in the bag
		
		Bag();
		Bag(int size);
		void insert(int v);
		int removeMin();
		int lookMin(); // This function will be used when merging two bags
		void reheapUp(int root, int last); // Restore heap structure after an insertion
		void reheapDown(int root, int bottom); // Restore heap structure after an removal
		Bag* merge(Bag *bag);  
		void print();  
		bool isEmpty();  
		//createLargerArray();
};

Bag::Bag() {
	elementsArray = new int[100];
	arraySize = 100;
	index = 0;
}


Bag::Bag(int size) {
	elementsArray = new int[size];
	arraySize = size;
	index = 0;
}

void Bag::insert(int v) {
	if(index < arraySize) {
		elementsArray[index] = v;
		reheapUp(0, index);
		index++;
	}
}
void Bag::reheapUp(int root, int last) {
	int parent;
	int temp;
	if(last > root) {
		 parent = (last-1) / 2;
          if(elementsArray[parent] > elementsArray[last])
          {
               // Swap elements
               temp = elementsArray[parent];
               elementsArray[parent] = elementsArray[last];
               elementsArray[last] = temp;
               // Make recursive call till reheaping completed
               reheapUp(root, parent);
          } 
	}
}

int Bag::removeMin() {
	int min;
     index--;
     // Copy last item into root
     min = elementsArray[0];
	 elementsArray[0] = elementsArray[index];
     // Reheap the tree
     reheapDown(0, index-1);
	 return min;
}

void Bag::reheapDown(int root, int bottom)
{
     int minChild;
     int rightChild;
     int leftChild;
	int temp;
     leftChild = root*2+1;
     rightChild = root*2+2;

     if(root < bottom){
          if(leftChild == bottom)
          {
               minChild = leftChild;
          }
          else
          {     // Get the one lowest in the tree (highest index in the array)
               if(elementsArray[leftChild] >= elementsArray[rightChild])
                    minChild = rightChild;
               else
                    minChild = leftChild;
          }
          if(elementsArray[root] > elementsArray[minChild])
          {
               // Swap these two elements
               temp = elementsArray[root];
               elementsArray[root] = elementsArray[minChild];
               elementsArray[minChild] = temp;
               // Make recursive call till reheaping completed
               reheapDown(root , bottom);
          }
     }
}

void Bag::print() {
    for(int i=0; i<index; i++){
         cout << elementsArray[i] << " ";	  
    }
	cout << endl;
}

bool Bag::isEmpty() {
	return index == 0 ? true : false;
}
int Bag::lookMin() {
	if(!this->isEmpty()) 
		return elementsArray[0];
}

Bag* Bag::merge(Bag *other) {
	Bag *temp = new Bag();
	int i = 0;
	while(!(this->isEmpty()) && !(other->isEmpty()) ) {
		/*cout << "itn " << i << endl;
		cout << "--------------" << endl;
		cout << "this " << this->lookMin() << endl;
		cout << "other " << other->lookMin() << endl;*/
		if(this->lookMin() == other->lookMin()) {
			int t = this->removeMin();
			other->removeMin();
			temp->insert(t);
		} else if(this->lookMin() < other->lookMin()) {
			int t = this->removeMin();
			temp->insert(t);
		} else if(this->lookMin() > other->lookMin()) {
			int t = other->removeMin();
			temp->insert(t);
		}
		i++;
	}
	// one of the two bags are empty
	while(!this->isEmpty()) {
		int t = this->removeMin();
		temp->insert(t);
	}
	while(!other->isEmpty()) {
		int t = other->removeMin();
		temp->insert(t);
	}
	return temp;
}

class Graph{
public:
    int nv;            // number of vertices
    int ne;            // number of edges
    int *nbr;          // array of neighbors of all vertices
    int *firstnbr;
    void graphFromEdgeList(int *tail, int *head);
    void printCSRGraph(); 
    void bfs(int s, int **levelp, int *nlevelsp, int **levelsizep, int **parentp);
    void walkCurrentLevel(); //assign levels to neighbors and add them to  next level bag
    
};

void Graph::graphFromEdgeList (int *tail, int* head) {
  int i, maxv = 0;
  // count vertices
  for (int e = 0; e < ne; e++) {
    if (tail[e] > maxv) 
      maxv = tail[e];
    if (head[e] > maxv) 
      maxv = head[e];
  }
  nv = maxv+1;
  nbr = new int[ne];
  fill_n(nbr, ne, 0);
  firstnbr = new int[nv+1];
  fill_n(firstnbr, nv+1, 0);
  // count neighbors of vertex v in firstnbr[v+1],
  for (int e = 0; e < ne; e++) 
    firstnbr[tail[e]+1]++;

  // cumulative sum of neighbors gives firstnbr[] values
  for (int v = 0; v < nv; v++) 
    firstnbr[v+1] += firstnbr[v];

  // pass through edges, slotting each one into the CSR structure
  for (int e = 0; e < ne; e++) {
    i = firstnbr[tail[e]]++;
    nbr[i] = head[e];
  }
  // the loop above shifted firstnbr[] left; shift it back right
  for (int v = nv; v > 0; v--) 
    firstnbr[v] = firstnbr[v-1];
  firstnbr[0] = 0;
}

void Graph::printCSRGraph () {
  int vlimit = 20;
  int elimit = 50;
  int e,v;
  cout << "\nGraph has " << nv << " vertices and" << ne << " edges.\n";
  cout << "firstnbr =";
  if (nv < vlimit) 
    vlimit = nv;
  for (v = 0; v <= vlimit; v++) 
    cout << " "<< firstnbr[v];
  if (nv > vlimit) 
    cout <<" ...";
  cout << "\nnbr =";
  if (ne < elimit) 
    elimit = ne;
  for (e = 0; e < elimit; e++) 
    cout << " "<<nbr[e];
  if (ne > elimit) 
    cout << " ...";
  cout << "\n\n";
}

void Graph::bfs (int s, int **levelp, int *nlevelsp, int **levelsizep, int **parentp) {
  int *level, *levelsize, *parent;
  int thislevel;
  int *queue, back, front;
  int i, v, w, e;
  level = *levelp = new int[nv];
  //fill_n(*levelp, nv+1, 0);
  
  levelsize = *levelsizep = new int[nv];
  //fill_n(*levelsizep, nv, 0);
  parent = *parentp = new int[nv];
  //fill_n(*parentp, nv, 0);
  queue = new int[nv];
  //fill_n(queue, nv, 0);

  // initially, queue is empty, all levels and parents are -1
  back = 0;   // position next element will be added to queue
  front = 0;  // position next element will be removed from queue
  for (v = 0; v < nv; v++) level[v] = -1;
  for (v = 0; v < nv; v++) parent[v] = -1;

  // assign the starting vertex level 0 and put it on the queue to explore
  thislevel = 0;
  level[s] = 0;
  levelsize[0] = 1;
  queue[back++] = s;

  // loop over levels, then over vertices at this level, then over neighbors
  while (levelsize[thislevel] > 0) {
    levelsize[thislevel+1] = 0;
    for (i = 0; i < levelsize[thislevel]; i++) {
      v = queue[front++];       // v is the current vertex to explore from
      for (e = firstnbr[v]; e < firstnbr[v+1]; e++) {
        w = nbr[e];          // w is the current neighbor of v
        if (level[w] == -1) {   // w has not already been reached
          parent[w] = v;
          level[w] = thislevel+1;
          levelsize[thislevel+1]++;
          queue[back++] = w;    // put w on queue to explore
        }
      }
    }
    thislevel = thislevel+1;
  }
  *nlevelsp = thislevel;
  delete[] queue;
}



int main (int argc, char* argv[]) {
  /*Graph *G = new Graph();
  int max_edges = 1000000;
  int *level, *levelsize, *parent;
  int *tail = new int[max_edges], *head = new int[max_edges];
  int nlevels;
  int from, to;
  int startvtx;
  int i, v, reached;
  int size=0;
  startvtx = 1;
  fill_n(head, max_edges, 0);
  fill_n(tail, max_edges, 0);
  cin >> from;
  cin >> to;
  while(!cin.eof()){
    tail[size] = from;
    head[size++] = to;
    cin >> from;
    cin >> to;
    //size++;
  }
  G->ne = size;
  cout << size << endl << endl;
 for(int i=0; i<G->ne; i++){
  cout << head[i] << " " << tail[i] <<endl;


  }
  G->graphFromEdgeList(tail, head);
  delete[] tail;
  delete[] head;
  G->printCSRGraph();
  cout << "Starting vertex for BFS is "<< startvtx << ".\n\n";
  G->bfs (startvtx, &level, &nlevels, &levelsize, &parent);
  reached = 0;
  for(i = 0; i<nlevels; i++)
    reached += levelsize[i];
  cout << "Breadth-first search from vertex " << startvtx << " reached ";
  cout << nlevels << " levels and " << reached << " vertices.\n";
  for(i = 0; i < nlevels; i++)
    cout << "level " << i << " vertices: " << levelsize[i] << endl;
  if(G->nv < 20){
    cout << "\n  vertex parent  level\n";
    for (v = 0; v < G->nv; v++) 
      cout << setw(6) << v << setw(7) << parent[v] << setw(7) << level[v] << endl;
  }*/
  // testing bag structure
	Bag *b1 = new Bag();
	Bag *b2 = new Bag();
	Bag *result = new Bag();
	
	b1->insert(20);
	b1->insert(11);
	b1->insert(13);
	b1->insert(4);
	b1->insert(77);
	b2->insert(1);
	b2->insert(111);
	b2->insert(4);
	b2->insert(8);
	b2->insert(9);
	b1->print();
	cout << "--------" << endl;
	b2->print();
	result = b1->merge(b2);
	cout << "--------" << endl;
	result->print();



	
  return 0;
}
