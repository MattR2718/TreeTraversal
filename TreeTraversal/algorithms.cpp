#include "algorithms.h"


/*
def findNeighbour(c, visited):
  r = None
  for n in graph[c]:
    if n not in visited:
      r = n
  return r

def dft(graph, start):
  visited = []
  stack = []
  visited.append(start) #add A to visited

  stack.append(start)
  while len(stack) > 0:
    nextNode = findNeighbour(stack[len(stack)-1], visited)
    if not nextNode == None:
      if nextNode not in visited:
        visited.append(nextNode)
      stack.append(nextNode)
    else:
      stack.pop()
    print(stack)
  return visited
*/

int findNeighbours(int& c, std::vector<int>& visited, std::map<int, std::vector<int>>& adjacencyList) {
    std::cout << c << '\n';
    if (c == 0) {
        std::cout << "0 IN NEIGHBOURS\n";
    }
    for (auto& n : adjacencyList[c]) {
        if (!std::count(visited.begin(), visited.end(), n)) {
            return n;
        }
    }
    return -1;
}

std::vector<int> depthFirst(std::map<int, std::vector<int>>& adjacencyList, int start)
{
    std::vector<int> visited;
    std::stack<int> stack;
    visited.push_back(start);
    stack.push(start);

    while (stack.size() > 0) {
        int nextNode = findNeighbours(stack.top(), visited, adjacencyList);
        //std::cout << nextNode << '\n';
        if (nextNode != -1) {
            if (nextNode) {
                visited.push_back(nextNode);
            }
            stack.push(nextNode);
        }
        else {
            stack.pop();
        }
        //std::cout << stack.size() << '\n';
        //std::cout << visited.size() << '\n';
        //std::cout << stack.top();
    }

	return visited;
}

std::vector<int> breadthFirst(std::map<int, std::vector<int>>& adjacencyList, int start)
{
	return std::vector<int>();
}
