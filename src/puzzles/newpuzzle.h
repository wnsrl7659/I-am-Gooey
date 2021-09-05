/******************************************************************************/
/*!
\file	newpuzzle.h
\author	Justice Mealer
\par    email:	j.mealer@digipen.edu
\par    Course: GAM250
\brief

\copyright    All content © 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#define R_FILE

#include <vector>
#include <list>
#include "../gameobject/gameobject.h"
#include "../messaging/messaging.h"
#include "../graphics/Sprite.h"
#include "../component/collider.h"
#include "../messaging/InputEvent.h"
#include "../vfx/LightManager.h"
#include "../component/controller.h"
#include "../component/MultiEmitter.h"

class DoorManager;
class PuzzleManager;
class Puzzle;
class PuzzleNode;

namespace PzManage
{
	extern PuzzleManager * pzm;
	extern DoorManager * dorm;
	void Init();
	void Update(float);
	void Shutdown();
};

class PuzzleEvent
{
public:
	enum State
	{
		inactive,
		active,
		failed,
		completed,
	};
	PuzzleEvent() { puzzleNumber = -1; currState = inactive; }
	PuzzleEvent(int n, State s) { puzzleNumber = n; currState = s; }
	~PuzzleEvent() {}
	int puzzleNumber;
	State currState;
	
};

class PuzzleManager
{
public:
	PuzzleManager();
	~PuzzleManager();
	void AddPuzzle(Puzzle*);
	void AddEmitter(MultiEmitter* me)
	{
		puzzcompemitters.push_back(me);
	}
	std::vector<MultiEmitter*> GetEmitters()
	{
		return puzzcompemitters;
	}
	void ActivatedNode(PuzzleNode*);
	void Update(float);
	std::vector<Puzzle*> *GetPuzzles();
	void EventCheck(PuzzleEvent*);
	void WorldEvents(std::string*);
	int CompletedPuzzles();
	void ResetCurrentPuzzle();
	void Clear();
	friend class PuzzleNode;
	friend class Puzzle;
	friend class PuzzleEvent;
	int numpuzzles;
	int hints;

	//Some getters for DebugWindow
	PuzzleNode* GetActiveNode() { return activeNode; }
	bool GetNodeIsActive() { return nodeIsActive; }
private:
	std::vector<Puzzle*> puzzles;
	std::vector<MultiEmitter*> puzzcompemitters;
	bool nodeIsActive;
	PuzzleNode* activeNode;
	
	void incorrect_movement();
	void complete_puzzle();
	bool check_puzzle_completed();
	
};

class Puzzle
{
public:
	Puzzle(std::string, int);
	~Puzzle();
	void AddNode(PuzzleNode*);
	bool Check(); //check if the puzzle is complete
	void Invalidate(); //turn off all nodes
	std::vector<PuzzleNode*> * GetNodes();
	friend class PuzzleNode;
	friend class PuzzleManager;
	friend class PuzzleEvent;
	bool isCompleted;
	int puzzleNumber;
private:
	std::vector<PuzzleNode*> nodes;
	PuzzleManager* parent;
	std::string name;
	
};

class PuzzleNode : public Component<PuzzleNode>
{
public:
	
	COMPONENT_NAME(PuzzleNode);
	PuzzleNode();
	~PuzzleNode();
	enum Type
	{
		cyan,
		magenta,
	};
	void Update(float);
	void Draw() const;
	void Init(Type);
	Puzzle* GetParent();

	void Activate();

	friend class PuzzleManager;
	friend class Puzzle;
	friend class PuzzleEvent;
	bool isStart;
	bool isActive;
	bool isCol;
private:
	
	Puzzle* parent;
	LightSource* light = nullptr;
	
	Type nodeType;
};

class DoorManager
{
public:
	DoorManager();
	~DoorManager();
	void WorldEvents(std::string*);
	void AddDoor(GameObject*);
	std::list<GameObject*> * GetDoors();
	void Check(PuzzleEvent*);
	void Clear();
	

private:
	std::list<GameObject*> doors;
};
