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
		//void remove();
		int lookMin(); // This function will be used when merging two bags
		void reheapUp(int root, int last); // Restore heap structure after an insertion
		//void reheapDown(); // Restore heap structure after an removal
		void merge(Bag *bag);  
		void print();  
};

Bag::Bag() {
	elementsArray = NULL;
	arraySize = 0;
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
/*
void Bag::remove() {
     index--;
     // Copy last item into root
     elementsArray[0] = elementsArray[index];
     // Reheap the tree
     reheapDown(0, index-1);
}

void Bag::reheapDown(int root, int bottom)
{
     int maxChild;
     int rightChild;
     int leftChild;

     leftChild = root*2+1;          // Get index of root's left child
     rightChild = root*2+2;          // Get index of root's right child

     // Check base case in recursive calls.  If leftChild's index is less
     // than or equal to the bottom index we have not finished recursively 
     // reheaping.
     if(leftChild <= bottom){
          if(leftChild == bottom)          // If this root has no right child then 
          {
               maxChild = leftChild;     //     leftChild must hold max key
          }
          else
          {     // Get the one lowest in the tree (highest index in the array)
               if(m_Elements[leftChild].getKey() <= m_Elements[rightChild].getKey())
                    maxChild = rightChild;
               else
                    maxChild = leftChild;
          }
          if(m_Elements[root].getKey() < m_Elements[maxChild].getKey())
          {
               // Swap these two elements
               temp = m_Elements[root];
               m_Elements[root] = m_Elements[maxChild];
               m_Elements[maxChild] = temp;
               // Make recursive call till reheaping completed
               ReheapDown(maxChild, bottom);
          }
     }
}
*/
void Bag::print() {
    for(int i=0; i<index; i++){
         cout << elementsArray[i] << std::endl;	  
    }
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
  Bag b(6);
  b.insert(12);
  b.insert(11);
  b.insert(17);
  b.insert(19);
  b.insert(3);
  b.print();
 
  return 0;
}
