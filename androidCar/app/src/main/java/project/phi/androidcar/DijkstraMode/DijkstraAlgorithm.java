package project.phi.androidcar.DijkstraMode;

public class DijkstraAlgorithm {
    // A utility function to find the vertex with minimum distance value,
    // from the set of vertices not yet included in shortest path tree
    int INF = 100;
    int[][] adjacencyMatrix = { { 0, INF, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0},
            { 1,  0,  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0,  1,  0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0},
            { 0,  0,  1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0,  0,  0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            { 0,  0,  0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2},
            { 0,  0,  0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            { 0,  0,  2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 2, 0, 0},
            { 0,  0,  0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
            {INF, 0,  0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0},
            { 0,  0,  0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0},
            { 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
            { 0,  0,  0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 1, 0},
            { 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
            { 0,  0,  0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0} };
    private static final int NO_PARENT = -1;
    // Function that implements Dijkstra's
    // single source shortest path
    // algorithm for a graph represented
    // using adjacency matrix
    // representation
    public void dijkstra(int startVertex)
    {
        int nVertices = adjacencyMatrix[0].length;

        // shortestDistances[i] will hold the
        // shortest distance from src to i
        int[] shortestDistances = new int[nVertices];

        // added[i] will true if vertex i is
        // included / in shortest path tree
        // or shortest distance from src to
        // i is finalized
        boolean[] added = new boolean[nVertices];

        switch(startVertex){
            case 3:
                adjacencyMatrix[3][2] = INF;
                break;
            case 6:
                adjacencyMatrix[6][7] = INF;
                break;
            case 13:
                adjacencyMatrix[13][12] = INF;
                break;
        }

        // Initialize all distances as
        // INFINITE and added[] as false
        for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++)
        {
            shortestDistances[vertexIndex] = Integer.MAX_VALUE;
            added[vertexIndex] = false;
        }

        // Distance of source vertex from
        // itself is always 0
        shortestDistances[startVertex] = 0;

        // Parent array to store shortest
        // path tree
        int[] parents = new int[nVertices];

        // The starting vertex does not
        // have a parent
        parents[startVertex] = NO_PARENT;

        // Find shortest path for all
        // vertices
        for (int i = 1; i < nVertices; i++)
        {

            // Pick the minimum distance vertex
            // from the set of vertices not yet
            // processed. nearestVertex is
            // always equal to startNode in
            // first iteration.
            int nearestVertex = -1;
            int shortestDistance = Integer.MAX_VALUE;
            for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++)
            {
                if (!added[vertexIndex] && shortestDistances[vertexIndex] < shortestDistance)
                {
                    nearestVertex = vertexIndex;
                    shortestDistance = shortestDistances[vertexIndex];
                }
            }

            // Mark the picked vertex as
            // processed
            added[nearestVertex] = true;

            // Update dist value of the
            // adjacent vertices of the
            // picked vertex.
            for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++)
            {
                int edgeDistance = adjacencyMatrix[nearestVertex][vertexIndex];

                if (edgeDistance > 0 && ((shortestDistance + edgeDistance) < shortestDistances[vertexIndex]))
                {
                    parents[vertexIndex] = nearestVertex;
                    shortestDistances[vertexIndex] = shortestDistance + edgeDistance;
                }
            }
        }

        if (startVertex != 0)
            pathToSource(startVertex, shortestDistances, parents);
        else
            printSolution(startVertex, shortestDistances, parents);
    }

    // A utility function to print
    // the constructed distances
    // array and shortest paths
    private static void printSolution(int startVertex, int[] distances, int[] parents)
    {
        int nVertices = distances.length;
        System.out.print("Vertex\t Distance\tPath");

        for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++)
        {
            if (vertexIndex != startVertex)
            {
                System.out.print("\n" + startVertex + " -> ");
                System.out.print(vertexIndex + " \t\t ");
                System.out.print(distances[vertexIndex] + "\t\t");
                printPath(vertexIndex, parents);
            }
        }
    }

    // Function to print shortest path
    // from source to currentVertex
    // using parents array
    private static void printPath(int currentVertex, int[] parents)
    {

        // Base case : Source node has
        // been processed
        if (currentVertex == NO_PARENT)
        {
            return;
        }
        printPath(parents[currentVertex], parents);
        System.out.print(currentVertex + " ");
    }

    private static void pathToSource(int startVertex, int[] distances, int[] parents)
    {
        int src = 0;
        System.out.print("Vertex\t Distance\tPath");
        System.out.print("\n" + startVertex + " -> ");
        System.out.print(src + " \t\t ");
        System.out.print(distances[src] + "\t\t");
        printPath(src, parents);
    }
}
