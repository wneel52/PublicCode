"""
UMass ECE 241 - Advanced Programming
Project 2 - Fall 2023
"""
import sys
from graph import Graph, Vertex
from priority_queue import PriorityQueue


class DeliveryService:
    def __init__(self) -> None:
        """
        Constructor of the Delivery Service class
        """
        self.city_map = Graph()
        self.MST = None  # Spanning Tree object -> will be assigned in function

    def buildMap(self, filename: str) -> None:
        """
        :param filename: takes in a text file representing a city map which will be used to make the graph object
        :return: None -> builds map using graph object
        """

        map_construct = open(filename, "r")  # opens file in read mode

        for line in map_construct:  # iterates over each line: given file must only have one connection per line
            reader = line.strip().split('|')

            starting_node = int(reader[0])  # Node A (Start)
            destination_node = int(reader[1])  # Node B (End)
            cost = int(reader[2])  # Cost to Travel

            self.city_map.addEdge(starting_node, destination_node, cost)
            # Will create a new vertex if one/both of are not present in the vertices list,
            # then puts a new edge between

    def isWithinServiceRange(self, restaurant: int, user: int, threshold: int) -> bool:
        """
        :param restaurant: Node to search from
        :param user: Destination of Delivery
        :param threshold: Maximum Distance from Restaurant
        :return: Bool -> True if in delivery range, false otherwise
        """

        if restaurant not in self.city_map.getVertices() or user not in self.city_map.getVertices():
            return False  # Conditional to check if we can travel between nodes

        distance = {}  # dictionary containing edge weights; sum is the distance traveled

        for Vers in self.city_map.getVertices():  # for all Vertices in the map
            if Vers == restaurant:  # Initialize distance of Restaurant as 0
                distance[Vers] = 0
            else:  # All other nodes have a distance of infinity
                distance[Vers] = float('Inf')

        for potential_paths in range(len(self.city_map.getVertices()) - 1):
            # Continue to iterate until shortest path is found
            for Vers in self.city_map:  # for each vertex in the map
                for nextVert in Vers.getConnections():  # gathers all possible connections
                    if (distance[Vers.id] != float("Inf") and distance[Vers.id] + Vers.getWeight(nextVert)
                            < distance[nextVert.id]):

                        distance[nextVert.id] = distance[Vers.id] + Vers.getWeight(nextVert)
                        # increment distance with new edge weight

        if distance[user] <= threshold:
            return True  # Returns ture if path distance from restaurant to user is less than threshold
        else:
            return False  # otherwise return false

    def buildMST(self, restaurant: int) -> bool:
        """
        :param restaurant: takes in a restaurant and creating a minimum spanning tree starting from the input node
        :return: None -> adds subtree to a list
        """
        if restaurant not in self.city_map.getVertices():  # check if restaurant object is in the map
            return False

        Start = self.city_map.getVertex(restaurant)  # get vert associated with restaurant int value
        pq = PriorityQueue()  # initialize Priority Queue Object to manage verts / edges

        for v in self.city_map:  # will iterate for each vertex in the map
            v.setDistance(float('Inf'))  # initialize all distances as infinity
            v.setPred(None)
        Start.setDistance(0)  # initialize "Start" or Restaurant Node to create graph from

        self.MST = Graph()  # initialize self.MST as a new graph object

        pq.buildHeap([(v.getDistance(), v) for v in self.city_map])
        while not pq.isEmpty():
            currentVert = pq.delMin()

            if currentVert.getPred() is not None and currentVert.getId() is not None:
                self.MST.addEdge(currentVert.getId(), currentVert.getPred().getId(), currentVert.getDistance())

            for nextVert in currentVert.getConnections():
                newCost = currentVert.getWeight(nextVert)

                if nextVert in pq and newCost < nextVert.getDistance():
                    nextVert.setPred(currentVert)
                    nextVert.setDistance(newCost)
                    pq.decreaseKey(nextVert, newCost)

    def minimalDeliveryTime(self, restaurant: int, user: int) -> int:
        """

        :param restaurant:
        :param user:
        :return:
        """
        if (user is not None and user in self.MST.getVertices()
                and restaurant is not None and restaurant in self.MST.getVertices()):

            pq = PriorityQueue()  # initialize PQ
            Start = self.MST.getVertex(restaurant)  # create a starting point for dijkstras
            Start.setDistance(0)  # set starting distance to 0
            pq.buildHeap([(v.getDistance(), v) for v in self.MST])  # build heap table
            visited = []

            while not pq.isEmpty():
                currentVert = pq.delMin()
                for nextVert in currentVert.getConnections():
                    newDist = currentVert.getDistance() + currentVert.getWeight(nextVert)

                    if newDist < nextVert.getDistance():
                        nextVert.setDistance(newDist)
                        nextVert.setPred(currentVert)
                        pq.decreaseKey(nextVert, newDist)
            if user is not None:
                path = []
                current = self.MST.getVertex(user)
                while current:
                    if current in visited:
                        break
                    visited.append(current)
                    path.insert(0, current.getId())
                    current = current.getPred()
                return int(self.MST.getVertex(user).getDistance())
            else:
                return - 1
        return -1

    def findDeliveryPath(self, restaurant: int, user: int) -> str:
        """
        :param restaurant: starting point for dijkstras algorithm
        :param user: end point used to extract path
        :return:
        """
        if restaurant not in self.city_map.getVertices() or user not in self.city_map.getVertices():
            return "INVALID"
        start = self.city_map.getVertex(restaurant)
        end = self.city_map.getVertex(user)

        for v in self.city_map:  # will iterate for each vertex in the map
            v.setDistance(float('Inf'))  # initialize all distances as infinity
            v.setPred(None)
        start.setDistance(0)  # initialize "Start" or Restaurant Node to create graph from

        pq = PriorityQueue()
        start.setDistance(0)
        pq.buildHeap([(v.getDistance(), v) for v in self.city_map])

        while not pq.isEmpty():
            currentVert = pq.delMin()
            for nextVert in currentVert.getConnections():
                newDist = currentVert.getDistance() + currentVert.getWeight(nextVert)

                if newDist < nextVert.getDistance():
                    nextVert.setDistance(newDist)
                    nextVert.setPred(currentVert)
                    pq.decreaseKey(nextVert, newDist)

        if end is not None:
            path = []
            current = end
            while current:
                path.insert(0, str(current.getId()))
                current = current.getPred()

            path_str = "->".join(path)

            return f"{path_str} ({end.getDistance()})"

    def findDeliveryPathWithDelay(self, restaurant: int, user: int, delay_info: dict[int, int]) -> str:
        """

        :param restaurant: node we begin search at, will take name "start" for the algorithm
        :param user: node we end search at, will take name "end" for the algorithm
        :param delay_info: dictionary containing time delay information
        :return: path information with delay added to the distance calculation
        """
        if restaurant not in self.city_map.getVertices() or user not in self.city_map.getVertices():
            return "INVALID"
        start = self.city_map.getVertex(restaurant)
        end = self.city_map.getVertex(user)

        for v in self.city_map:  # will iterate for each vertex in the map
            v.setDistance(float('Inf'))  # initialize all distances as infinity
            v.setPred(None)
        start.setDistance(0)  # initialize "Start" or Restaurant Node to create graph from

        pq = PriorityQueue()
        pq.buildHeap([(v.getDistance(), v) for v in self.city_map])

        while not pq.isEmpty():
            currentVert = pq.delMin()
            for nextVert in currentVert.getConnections():
                newDist = currentVert.getDistance() + currentVert.getWeight(nextVert)

                if newDist < nextVert.getDistance():
                    nextVert.setDistance(newDist)
                    nextVert.setPred(currentVert)
                    pq.decreaseKey(nextVert, newDist)

        if end is not None:

            path = []
            current = end
            inc_dist = 0

            while current:
                if current.getId() in delay_info:
                    inc_dist += delay_info[current.getId()]

                path.insert(0, str(current.getId()))
                current = current.getPred()

            path_str = "->".join(path)

            return f"{path_str} ({end.getDistance() + inc_dist})"

    ## DO NOT MODIFY CODE BELOW!
    @staticmethod
    def nodeEdgeWeight(v):
        return sum([w for w in v.connectedTo.values()])

    @staticmethod
    def totalEdgeWeight(g):
        return sum([DeliveryService.nodeEdgeWeight(v) for v in g]) // 2

    @staticmethod
    def checkMST(g):
        for v in g:
            v.color = 'white'

        for v in g:
            if v.color == 'white' and not DeliveryService.DFS(g, v):
                return 'Your MST contains circles'
        return 'MST'

    @staticmethod
    def DFS(g, v):
        v.color = 'gray'
        for nextVertex in v.getConnections():
            if nextVertex.color == 'white':
                if not DeliveryService.DFS(g, nextVertex):
                    return False
            elif nextVertex.color == 'black':
                return False
        v.color = 'black'

        return True

# NO MORE TESTING CODE BELOW!
# TO TEST YOUR CODE, MODIFY test_delivery_service.py
