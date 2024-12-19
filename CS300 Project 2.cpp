// CS300 Project 2.cpp 
// Class: CS300
// Author: Westley Hunter

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>

class Course {
private:
	
public:
	std::string courseAbr;
	std::string courseName;
	std::vector<std::string> coursePrereqs;
	Course() {}
	void addPrereq(std::string);
};

void Course::addPrereq(std::string prereq) {
	coursePrereqs.push_back(prereq);
}

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course newCourse) : Node() {
		course = newCourse;
	}
};

class BinarySearchTree {
private:
	Node* root;

	void inOrder(Node* node);
	Node* removeNode(Node* node, Node* prev, std::string courseAbr);

public:
	BinarySearchTree();
	void InOrder();
	void addNode(Course course);
	void RemoveNode(std::string courseAbr);
	Course search(std::string courseAbr);
	void printCourse(Course course);
};

//Default Constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//Public function to print courses in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

//Private helper function to print courses in order
void BinarySearchTree::inOrder(Node* node) {
	
	if (node != nullptr){
		inOrder(node->left);
		printCourse(node->course);
		inOrder(node->right);
	}
}

//Public function to add a node to the BST
void BinarySearchTree::addNode(Course course) {
	if (root == nullptr) {
		root = new Node(course);
		return;
	}
	else {
		Node* curr = root;
		while (true) {
			if (curr->course.courseAbr > course.courseAbr) {
				if (curr->left == nullptr) {
					curr->left = new Node(course);
					return;
				}
				else {
					curr = curr->left;
					}
			}
			else {
				if (curr->right == nullptr) {
					curr->right = new Node(course);
					return;
				}
				else {
					curr = curr->right;
				}
			}
		}
	}
}

//Public function to remove a node
void BinarySearchTree::RemoveNode(std::string courseAbr) {
	removeNode(root, nullptr, courseAbr);
}

//Private function to remove a node from the BST
Node* BinarySearchTree::removeNode(Node* node, Node* prev, std::string courseAbr) {
	if (node == nullptr) {
		return node;
	}
	else if ((node->course.courseAbr == courseAbr) && (node == root)) { //If matching node for removal is the root
		if (node->course.courseAbr == courseAbr) { //If matching node is found
			if (node->left == nullptr && node->right == nullptr) { //No children
				delete node;
			}
			else if (node->left != nullptr && node->right == nullptr) { //Left child only
				Node* temp = node;
				root = node->left;
				delete temp;
			}
			else if (node->left == nullptr && node->right == nullptr) { //Right child only 
				Node* temp = node;
				root = node->right;
				delete temp;
			}
			else { //Both children
				Node* nextNode = node->right;
				while (nextNode->left != nullptr) {
					nextNode = nextNode->left;
				}
				root->course = nextNode->course;
				root->right = removeNode(node->right, node, nextNode->course.courseAbr);
			}
		}
	} //Removing node if the matched node is the root
	else {
		if (node->course.courseAbr == courseAbr) { //If matching node is found & not root
			if (node->left == nullptr && node->right == nullptr) { //No children
				if (prev->left == node) { 
					prev->left = nullptr;
				}
				else {
					prev->right = nullptr;
				}
				std::cout << "none";
				delete node;
			}
			else if (node->left != nullptr && node->right == nullptr) { //Left child only
				Node* temp = node;
				node = node->left;
				std::cout << "left";
				if (prev->left == node) {
					prev->left = node;
				}
				else {
					prev->right = node;
				}
				delete temp;
			}
			else if (node->left == nullptr && node->right == nullptr) { //Right child only 
				Node* temp = node;
				node = node->right;
				std::cout << "right";
				if (prev->left == node) {
					prev->left = node;
				}
				else {
					prev->right = node;
				}
				delete temp;
			}
			else { //Both children
				Node* nextNode = node->right;
				std::cout << "both";
				while (nextNode->left != nullptr) {
					nextNode = nextNode->left;
				}
				
				node->course = nextNode->course;
				node->right = removeNode(node->right, node, nextNode->course.courseAbr);
			}
		}
		else if (node->course.courseAbr > courseAbr) { //Traversing down left child
			removeNode(node->left, node, courseAbr);
		}
		else { //Traversing down right child
			removeNode(node->right, node, courseAbr);
		}
	}
	return node;
}

//Publc function to search and return a Course
Course BinarySearchTree::search(std::string courseAbr) {
	Node* curr = root;
	while (curr != nullptr) {
		if (curr->course.courseAbr == courseAbr) { //If matching node is found, return
			return curr->course;
		}
		else if (curr->course.courseAbr > courseAbr) { //If id is greater than target id, go down left tree
			curr = curr->left;
		}
		else { //Moving down right tree
			curr = curr->right;
		}
	}

	//If matching course is not found, then return a empty course
	Course course;
	return course;
}
//Public function to print out course information
void BinarySearchTree::printCourse(Course course) { 
	std::cout << course.courseAbr << ": " << course.courseName
		<< " | Course Prerequisites: ";
	if (course.coursePrereqs.size() == 0) { //Prints none for empty prereq vectors
		std::cout << "None\n";
		return;
	}
	else { //Prints out each prereq
		for (int i = 0; i < course.coursePrereqs.size(); i++) {
			if ((i + 1) >= course.coursePrereqs.size()) {
				std::cout << course.coursePrereqs.at(i) << std::endl;
				return;
			}
			std::cout << course.coursePrereqs.at(i) << ", ";
		}
		std::cout << std::endl;
		return;
	}
}

//Public function to load CSV into bst
void loadCSV(std::string csvFilePath, BinarySearchTree* bst) { 
	
	std::ifstream inFS;
	inFS.open(csvFilePath.c_str()); //Opens csv file
	
	//Declaring variables to hold info from file
	std::string dataLine = ""; 
	std::string courseName = "";
	std::vector<std::string> data;

	if (inFS.is_open()) { //CHecks to ensure file is opened
		
		while (inFS.good()) { //Loops through csv until file is empty
			getline(inFS, dataLine); //Gets csv line
			std::stringstream dataStream(dataLine); 

			if (dataLine.empty()) { //Checks to see if line is empy
				continue;
			}

			while (getline(dataStream, courseName, ',')) { //Gathers each data point from line and pushs it into a vector
				data.push_back(courseName);
			}
		
			//Creates new course and adds info in from data vector
			Course newCourse = Course();
			newCourse.courseAbr = data[0];
			newCourse.courseName = data[1];
			for (int i = 2; i < data.size(); ++i) { //Adds each prerequ into prereq vector
				if (!data[i].empty()) {
					newCourse.addPrereq(data[i]);
				}
			}

			bst->addNode(newCourse); //Adds course into bst
			data.clear();
		}
		inFS.close(); //Closes file
	}
	else { //If file does not open
		std::cout << "File could not be opened.\n";
	}
}

//Function to print the menu options
static void printMenuOptions() { 
	std::cout << std::endl << "Welcome to the course planner.\n"
		<< "    1. Load Data Structure.\n"
		<< "    2. Print Course List.\n"
		<< "    3. Print Course.\n"
		<< "    4. Remove Course.\n"
		<< "    9. Exit.\n\n "
		<< "What would you like to do? ";
}

int main(){
	BinarySearchTree* bst = new BinarySearchTree(); //Creates instance of BST

	int choice = 0; //Inits choice variable for main menu loop
	int incorrect = 0; //Safeguard to ensure that multiple wrong answers cannot be answered
	std::string courseChoice; //Variable for choosing courses
	Course course; //Helped Course to print out information
	while ( (choice != 9) && (incorrect < 3) ) { //Main Menu loop
		printMenuOptions(); //Prints menu options
		std::cin >> choice; //Gathers input
		

		switch (choice) { //Easily determines which choice was made
		case 1: //Loading data in
			std::cout << "Loading data...\n";
			loadCSV("CS 300 ABCU_Advising_Program_Input.csv", bst);
			break;
		case 2: //Prints schedule out in order
			std::cout << "\nHere is a sample schedule:\n";
			bst->InOrder();
			break;
		case 3: //Prints a singular course
			std::cout << "\nWhat course do you want to know about? ";
			std::cin >> courseChoice;
			course = bst->search(courseChoice);
			if (!course.courseAbr.empty()) { //Checks to ensure that the course was found
				std::cout << "\n\n";
				bst->printCourse(course);
			}
			else { //If course not found, says so
				std::cout << "\n\nCourse " << courseChoice << " could not be found.\n";
			}
			break;
		case 4: //Removes a course
			std::cout << "\nWhat course do you want to remove? ";
			std::cin >> courseChoice;
			bst->RemoveNode(courseChoice);
			std::cout << "Course " << courseChoice << " removed.\n";
			break;
		
		default: //Runs if invlaid input was recieved
			std::cout << choice << " is not a valid option. Please try again.\n";
			incorrect = incorrect + 1;
		}
	}
	std::cout << "Goodbye!\n";

	return 0;
}

