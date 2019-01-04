#include "Node.h"
#include "Track.h"
#include "Vehicle.h"
#include <iostream>
#include <algorithm>

Node::Node(): mTrack(nullptr)
{

}

Node::Node(Track* track) : mTrack(track)
{
}

Node::~Node()
{
	
	for (int i = 0; i < mChildren.size(); i++) 
	{
		if (mChildren[i] != NULL) 
		{
			mChildren[i]->~Node();
		}
	}

	for (std::vector<Node*>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		delete (*it);
	}
	mChildren.clear();
}

Node* Node::AppendChild(Node* child)
{
	child->SetParent(this);
	mChildren.push_back(child);
	return mChildren.back();
}

void Node::SetParent(Node* parent)
{
	mParent.push_back(parent);
}

void Node::PopBackChild()
{
	mChildren.pop_back();
}

void Node::RemoveChild(int position)
{
	if (mChildren.size() > 0) 
	{
		mChildren.erase(mChildren.begin() + position);
	}
	else 
	{
		mChildren.pop_back();
	}
}

bool Node::HasChildren() const
{
	if (mChildren.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Node::HasParent() const
{
	if (!mParent.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

Node* Node::GetChild(int position) const
{
	if (mChildren.size() < position)
	{
		return nullptr;
	}
	else
	{
		return mChildren[position];
	}
}

int Node::CountNodesRecursively(Node *root, int& count)
{
	Node *parent = root;
	Node *child = NULL;

	for (int it = 0; it < parent->GetChildrenNumber(); it++)
	{
		child = parent->GetChild(it);
		count++;
		if (child->GetChildrenNumber() > 0)
		{
			CountNodesRecursively(child, count);
		}
	}

	return count;
}

int Node::GetGrandChildrenNumber()
{
	int t = 0;

	if (mChildren.size() < 1)
	{
		return 0;
	}

	CountNodesRecursively(this, t);

	return t;
}

Node* Node::FindChild(int id)
{
	for (int i = 0; i < mChildren.size(); ++i)
	{
		if (mChildren[i]->GetTrack()->GetID() == id)
		{
			return mChildren[i];
		}

		auto find_child = mChildren[i]->FindChild(id);
		if (find_child != nullptr)
		{
			return find_child;
		}
	}
	return nullptr;
}

void Node::Print() const
{
	
	if (mTrack != nullptr)
	{
		std::cout << mTrack->GetID() << " ";
	}
	
	if (!mChildren.empty())
	{
		std::cout << "(";
		for (const auto& child : mChildren)
		{
			child->Print();
		}
		std::cout << ")";
	}
}

bool Node::SortByNumberOfChildren()
{
	if (mChildren.empty())
	{
		return false;
	}

	for (const auto& child : mChildren)
	{
		child->SortByNumberOfChildren();
	}

	std::sort(mChildren.begin(), mChildren.end(), CompareByNumberOfChildren());

	return true;
}

bool Node::AddVehicleToTrack(Vehicle* vehicle, bool continue_down)
{
	if (mTrack != nullptr)
	{
		// Checking if parents are blocking the child.
		std::vector<Node*> parents = GetParents();
		if (!parents.empty())
		{
			for (Node* parent : parents)
			{
				if (parent->GetTrack() != nullptr)
				{
					std::vector<Vehicle*> parkedVehicles = parent->GetTrack()->GetParkedVehicles();
					if (!parkedVehicles.empty())
					{
						for (Vehicle* v : parkedVehicles)
						{
							if (vehicle->GetDepartureTime() < v->GetDepartureTime())
							{
								return false;
							}
						}
					}
				}
			}
		}
		// Checking if the child is blocking its children.
		std::vector<Node*> children = GetChildren();
		if (!children.empty())
		{
			for (Node* child : children)
			{
				if (child->GetTrack() != nullptr)
				{
					std::vector<Vehicle*> parkedVehicles = child->GetTrack()->GetParkedVehicles();
					if (!parkedVehicles.empty())
					{
						for (Vehicle* v : parkedVehicles)
						{
							if (vehicle->GetDepartureTime() > v->GetDepartureTime())
							{
								return false;
							}
						}
					}
				}
			}
		}
		if (GetTrack()->ParkVehicle(vehicle))
		{
			return true;
		}
	}

	if (!continue_down) {
		return false;
	}

	if (!mChildren.empty())
	{
		for (const auto& child : mChildren)
		{
			if (child->AddVehicleToTrack(vehicle))
			{
				return true;
			}
		}
	}

	return false;
}

bool Node::CanSwitchVehicleInTrack(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	if (mTrack != nullptr)
	{
		// Checking if parents are blocking the child.
		std::vector<Node*> parents = GetParents();
		if (!parents.empty())
		{
			for (Node* parent : parents)
			{
				if (parent->GetTrack() != nullptr)
				{
					std::vector<Vehicle*> parkedVehicles = parent->GetTrack()->GetParkedVehicles();
					if (!parkedVehicles.empty())
					{
						for (Vehicle* v : parkedVehicles)
						{
							if (newVehicle->GetDepartureTime() < v->GetDepartureTime())
							{
								return false;
							}
						}
					}
				}
			}
		}

		// Checking if the child is blocking its children.
		std::vector<Node*> children = GetChildren();
		if (!children.empty())
		{
			for (Node* child : children)
			{
				if (child->GetTrack() != nullptr)
				{
					std::vector<Vehicle*> parkedVehicles = child->GetTrack()->GetParkedVehicles();
					if (!parkedVehicles.empty())
					{
						for (Vehicle* v : parkedVehicles)
						{
							if (newVehicle->GetDepartureTime() > v->GetDepartureTime())
							{
								return false;
							}
						}
					}
				}
			}
		}

		if (GetTrack()->CanSwitchVehicles(oldVehicle, newVehicle))
		{
			return true;
		}
	}
}

bool Node::SwitchVehicleInTrack(Vehicle* oldVehicle, Vehicle* newVehicle)
{
	if (CanSwitchVehicleInTrack(oldVehicle, newVehicle))
	{
		if (GetTrack()->SwitchVehicles(oldVehicle, newVehicle)) {
			return true;
		}
	}
	return false;
}
