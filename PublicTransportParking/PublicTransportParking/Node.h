#pragma once

#include<vector>

class Node
{

public:
	Node();
	Node(class Track* track);
	~Node();

	Node* AppendChild(Node *child);
	void SetParent(Node *parent);
	void PopBackChild();
	void RemoveChild(int position);
	Node* FindChild(int id);

	bool HasChildren() const;
	bool HasParent() const;

	std::vector<Node*> GetParents() const { return mParent; }
	std::vector<Node*> GetChildren() const { return mChildren; }
	Node* GetChild(int position) const;
	int GetChildrenNumber() const { return mChildren.size(); }
	int GetGrandChildrenNumber();
	class Track* GetTrack() { return mTrack; }

	void Print() const;
	bool SortByNumberOfChildren();
	bool AddVehicleToTrack(class Vehicle* vehicle, bool continue_down = true);
	bool CanSwitchVehicleInTrack(class Vehicle* oldVehicle, class Vehicle* newVehicle);
	bool SwitchVehicleInTrack(class Vehicle* oldVehicle, class Vehicle* newVehicle);

	struct CompareByNumberOfChildren
	{
		bool operator()(const Node* a, const Node* b)
		{
			return (a->GetChildrenNumber() < b->GetChildrenNumber());
		}
	};

private:
	class Track* mTrack;
	std::vector<Node*> mParent;
	std::vector<Node*> mChildren;
	int CountNodesRecursively(Node *root, int& count);
};