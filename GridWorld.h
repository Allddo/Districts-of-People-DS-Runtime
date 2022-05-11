
#ifndef _GRID_WORLD_H
#define _GRID_WORLD_H

#include <vector>
#include <iostream>

using std::vector;


class GridWorld {

  private:

    struct Node{
      int memberID;
      Node* prev;
      Node* next;

    };

    struct district{
      int districtPop;
      Node *memberFront;
      Node *memberBack;
      
    };

    struct peopleInfo{

      int row;
      int column;
      bool is_alive;
      Node *backDoor;

    };

    vector<vector<district> > theGrid;

    vector<peopleInfo> people;

    district deadPool;

    //counter used in births and deaths
    int totalPop;

    int gridRows;

    int gridCols;
    

  public:
    /**
    * constructor:  initializes a "world" with nrows and
    *    ncols (nrows*ncols districtcs) in which all 
    *    districtricts are empty (a wasteland!).
    */
    GridWorld(unsigned nrows, unsigned ncols)   {

      theGrid = vector<vector<district> > (nrows, vector<district>(ncols));
      this->gridRows = nrows;
      this->gridCols = ncols;
      deadPool.memberFront = nullptr;
      deadPool.memberBack = nullptr;
      totalPop = 0;

    }
    ~GridWorld(){
    }

    /*
     * function: birth
     * description:  if row/col is valid, a new person is created
     *   with an ID according to rules in handout.  New person is
     *   placed in district (row, col)
     *
     * return:  indicates success/failure
     */
    bool birth(int row, int col, int &id){
      
      if(row > gridRows || col > gridCols || row < 0 || col < 0){
        return false;
      }
      //if list is empty 
      if(theGrid[row][col].memberFront == nullptr){
        theGrid[row][col].districtPop = 0;

        Node* mem = new Node();
        mem->prev = nullptr;
        mem->next = nullptr;

        if(deadPool.memberFront == nullptr){
          mem->memberID = totalPop;
          id = mem->memberID;
        }
        else{
          mem->memberID = deadPool.memberFront->memberID;
          id = mem->memberID;
          Node* temp = deadPool.memberFront;
          deadPool.memberFront = deadPool.memberFront->next;
          delete(temp);
          deadPool.memberFront->prev = nullptr;
        }
      
        theGrid[row][col].memberFront = mem;
        theGrid[row][col].memberBack = mem;

        people.push_back(peopleInfo());
        // std::cout << people.size() << std::endl;
        
        people[totalPop].column = col;
        people[totalPop].row = row;
        people[totalPop].is_alive = true;

        people[totalPop].backDoor = mem;

      }
      //if not empty
      else if(theGrid[row][col].memberFront->prev == nullptr && theGrid[row][col].memberFront->next == nullptr){

        Node* mem = new Node();
        mem->next = nullptr;
        mem->prev = theGrid[row][col].memberFront;

        if(deadPool.memberFront == nullptr){
          mem->memberID = totalPop;
          id = mem->memberID;
        }
        else{
          mem->memberID = deadPool.memberFront->memberID;
          id = mem->memberID;
          Node* temp = deadPool.memberFront;
          deadPool.memberFront = deadPool.memberFront->next;
          delete(temp);
          deadPool.memberFront->prev = nullptr;
        }

        theGrid[row][col].memberFront->next = mem;
        theGrid[row][col].memberBack = mem;

        people.push_back(peopleInfo());
        people[totalPop].column = col;
        people[totalPop].row = row;
        people[totalPop].is_alive = true;

        people[totalPop].backDoor = mem;

      }
      else{

        Node* mem = new Node();

        if(deadPool.memberFront == nullptr){
          mem->memberID = totalPop;
          id = mem->memberID;
        }
        else{
          mem->memberID = deadPool.memberFront->memberID;
          id = mem->memberID;
          
          Node* temp = deadPool.memberFront;
          deadPool.memberFront = deadPool.memberFront->next;
          delete(temp);
          
          // deadPool.memberFront->prev = nullptr;
          
        }
        
        theGrid[row][col].memberBack->next = mem;
        theGrid[row][col].memberBack->next->prev =theGrid[row][col].memberBack; 
        theGrid[row][col].memberBack = mem;
        theGrid[row][col].memberBack->next = nullptr;

        people.push_back(peopleInfo());
        people[totalPop].column = col;
        people[totalPop].row = row;
        people[totalPop].is_alive = true;

        people[totalPop].backDoor = theGrid[row][col].memberBack;

      }
      theGrid[row][col].districtPop++;
      totalPop++;
      return true;
    }

    /*
     * function: death 
     * description:  if given person is alive, person is killed and
     *   data structures updated to reflect this change.
     *
     * return:  indicates success/failure
     */
    bool death(int personID){

      if(people[personID].is_alive == false){
        return false;
      }

      if(people[personID].backDoor->prev == nullptr && people[personID].backDoor->next == nullptr){

         theGrid[people[personID].row][people[personID].column].memberFront = nullptr;
         theGrid[people[personID].row][people[personID].column].memberBack = nullptr;
         people[personID].is_alive = false;
         delete(people[personID].backDoor);
        
      }
      else if(people[personID].backDoor->prev == nullptr){
         
         Node* temp = people[personID].backDoor;
         people[personID].backDoor = people[personID].backDoor->next;
         theGrid[people[personID].row][people[personID].column].memberFront = people[personID].backDoor;
         people[personID].backDoor->prev = nullptr;
        
         people[personID].is_alive = false;
         delete(temp); 
         
      }
      else if(people[personID].backDoor->prev != nullptr && people[personID].backDoor->next != nullptr){

         Node* temp = people[personID].backDoor->prev;
         temp->next = people[personID].backDoor->next;
         people[personID].backDoor->next->prev = temp;
         people[personID].is_alive = false;
         delete(people[personID].backDoor);
        
      }
      else if(people[personID].backDoor->next == nullptr){

         people[personID].backDoor->prev->next = nullptr;
         theGrid[people[personID].row][people[personID].column].memberBack = people[personID].backDoor->prev;
         people[personID].is_alive = false;
         delete(people[personID].backDoor);
  
      }
      else{
        return false;
      }

      //move to dealpool list
      if(deadPool.memberFront == nullptr){
        Node* newNode = new Node();
        newNode->memberID = personID;
        deadPool.memberFront = newNode;
        deadPool.memberBack = newNode;
        
      }
      else{
        Node* newNode = new Node();
        newNode->memberID = personID;
        deadPool.memberBack->next = newNode;
        deadPool.memberBack->next->prev = deadPool.memberBack;
        deadPool.memberBack = newNode;
        deadPool.memberBack->next = nullptr;
      }
      totalPop--;
      theGrid[people[personID].row][people[personID].column].districtPop--;
      return true;
    }

    /*
     * function: whereis
     * description:  if given person is alive, his/her current residence
     *   is reported via reference parameters row and col.
     *
     * return:  indicates success/failure
     */
    bool whereis(int id, int &row, int &col)const{
        
      if(id > people.size()|| id < 0){
        return false;
      }
      else if(people[id].is_alive == true){
        row = people[id].row;
        col = people[id].column;
        return true;
      }
      else{
        return false;
      }
    }
    /*
     * function: move
     * description:  if given person is alive, and specified target-row
     *   and column are valid, person is moved to specified district and
     *   data structures updated accordingly.
     *
     * return:  indicates success/failure
     *
     * comment/note:  the specified person becomes the 'newest' member
     *   of target district (least seniority) --  see requirements of members().
     */
    bool move(int id, int targetRow, int targetCol){

      if(people[id].is_alive == false || targetRow > gridRows || targetCol > gridCols|| targetRow < 0 || targetCol < 0){
        return false;
      }

      if(targetRow == people[id].row && targetCol == people[id].column){
          
          theGrid[targetRow][targetCol].memberBack->next = theGrid[targetRow][targetCol].memberFront;
          theGrid[targetRow][targetCol].memberFront->prev = theGrid[targetRow][targetCol].memberBack;
          theGrid[targetRow][targetCol].memberFront = theGrid[targetRow][targetCol].memberFront->next;
          theGrid[targetRow][targetCol].memberFront->prev = nullptr;
          theGrid[targetRow][targetCol].memberBack->next->next = nullptr;
          return true;
      }
      
      
      if(people[id].backDoor->prev == nullptr && people[id].backDoor->next == nullptr){

         theGrid[people[id].row][people[id].column].memberFront = nullptr;
         theGrid[people[id].row][people[id].column].memberBack = nullptr;
         
         delete(people[id].backDoor);
        //  people[id].backDoor = nullptr;

      }
      else if(people[id].backDoor->prev == nullptr){
         
         Node* temp = people[id].backDoor;
         people[id].backDoor = people[id].backDoor->next;
         theGrid[people[id].row][people[id].column].memberFront = people[id].backDoor;
         people[id].backDoor->prev = nullptr;
        //  people[id].backDoor->next = nullptr;
         delete(temp); 
      }
      else if(people[id].backDoor->next == nullptr){
         people[id].backDoor->prev->next = nullptr;
         theGrid[people[id].row][people[id].column].memberBack = people[id].backDoor->prev;
         
         delete(people[id].backDoor);
      }
      else{
         Node* temp = people[id].backDoor->prev;
         temp->next = people[id].backDoor->next;
         people[id].backDoor->next->prev = temp;
         
         delete(temp);
        
        //  people[id].backDoor = nullptr;
      }
      
      //empty
      if(theGrid[targetRow][targetCol].memberFront == nullptr){
        
        Node* temp = new Node();
        temp->memberID = id;
        temp->prev = nullptr;
        temp->next = nullptr;
        
        theGrid[targetRow][targetCol].memberFront = temp;
        
      }
      //at back
      else{
        Node* temp = new Node();
        
        temp->memberID = id;
        temp->prev = nullptr;
        temp->next = nullptr;
        
        theGrid[targetRow][targetCol].memberBack->next = temp;
        theGrid[targetRow][targetCol].memberBack->next->prev = theGrid[targetRow][targetCol].memberBack;
        theGrid[targetRow][targetCol].memberBack = temp;
        theGrid[targetRow][targetCol].memberBack->next = nullptr;
        
      }

      people[id].row = targetRow;
      people[id].column = targetCol;
      
      theGrid[people[targetRow].row][people[targetCol].column].districtPop++;
      theGrid[people[id].row][people[id].column].districtPop--;
      
      return true;

    }

    std::vector<int> * members(int row, int col)const{
      
      Node* mem = theGrid[row][col].memberFront;
      vector<int> * temp = new vector<int>;

      if(mem == nullptr){
        return temp;
      }
      else{
      
        while(mem != nullptr){
          temp->push_back(mem->memberID);
          mem = mem->next;
        }
        return temp;
      }
      
    }

    /*
     * function: population
     * description:  returns the current (living) population of the world.
     */
    int population()const{
      return totalPop;
    }
    
    /*
     * function: population(int,int)
     * description:  returns the current (living) population of specified
     *   district.  If district does not exist, zero is returned
     */
    int population(int row, int col)const{
      if(row > gridRows || col > gridCols){
        return 0;
      }
      else{
        return theGrid[row][col].districtPop;
      }
    }

    /*
     * function: num_rows
     * description:  returns number of rows in world
     */
    int num_rows()const {
      return gridRows;
    }

    /*
     * function: num_cols
     * description:  returns number of columns in world
     */
    int num_cols()const {
      return gridCols;
    }



};

#endif

