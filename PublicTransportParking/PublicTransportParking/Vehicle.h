#pragma once

#include<vector>

class Vehicle
{
public:
	Vehicle();
	~Vehicle();

	void SetLength(int length) { mLength = length; }
	void SetCategory(int category) { mCategory = category; }
	void SetCanPark(std::vector <std::string> canPark);
	void SetDepartureTime(int departureTime) { mDepartureTime = departureTime; }
	void SetSchedule(int schedule) { mSchedule = schedule; }
	void SetTrackID(int trackID) { mTrackID = trackID; }

	int GetLength() const { return mLength; }
	int GetCategory() const { return mCategory; }
	std::vector<int> GetCanPark() const { return mCanPark; }
	bool GetCanParkOnTrack(int trackNumber) const { return mCanPark.at(trackNumber); }
	int GetDepartureTime() const { return mDepartureTime; }
	int GetSchedule() const { return mSchedule; }
	int GetTrackID() const { return mTrackID; }

	struct CompareByDepartureTime
	{
		bool operator()(const Vehicle* a, const Vehicle* b)
		{
			return (a->GetDepartureTime() < b->GetDepartureTime());
		}
	};

private:
	int mLength;
	int mCategory;
	int mDepartureTime;
	int mSchedule;
	int mTrackID;
	std::vector<int> mCanPark;
};

inline bool operator==(const Vehicle& lhs, const Vehicle& rhs)
{
	return lhs.GetLength() == rhs.GetLength() && lhs.GetDepartureTime() == rhs.GetDepartureTime() && lhs.GetCategory() == rhs.GetCategory() && lhs.GetSchedule() == rhs.GetSchedule();
}
