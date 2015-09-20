#pragma once
#include <vector>
#include <glm\glm.hpp>

using namespace glm;
using namespace std;

namespace GameEngine3D
{

	struct Point
	{
		float x;
		float y;

		Point(float xval = 0, float yval = 0)
		{
			x = xval;
			y = yval;
		}
	};

	struct AABB
	{
		Point center;
		Point halfSize;

		AABB(Point c = Point(), Point hf = Point())
		{
			center = c;
			halfSize = hf;
		};

		bool contains(Point a)
		{
			if ((a.x <= center.x + halfSize.x) && (a.x >= center.x - halfSize.x))
			{
				if ((a.y <= center.y + halfSize.y) && (a.y >= center.y - halfSize.y))
				{
					return true;
				}
			}
			return false;
		}

		bool intersects(AABB other, float &xPos, float &yPos)
		{

			if ((abs(center.x - other.center.x) < (halfSize.x + other.halfSize.x)) && (abs(center.y - other.center.y) < (halfSize.y + other.halfSize.y))) //is collision?
			{
				if (abs(center.x - other.center.x) > abs(center.y - other.center.y))
				{
					if (center.x < other.center.x) //x-direction collision
					{
						//player is left of wall
						xPos = (center.x + halfSize.x) - (other.center.x - other.halfSize.x);
					}
					else
					{
						//player is right of wall
						xPos = (center.x - halfSize.x) - (other.center.x + other.halfSize.x);
					}
				}
				else
				{
					if (center.y < other.center.y) //y-direction collision
					{
						//player is front wall
						yPos = (center.y + halfSize.y) - (other.center.y - other.halfSize.y);
					}
					else
					{
						//player is behind wall
						yPos = (center.y - halfSize.y) - (other.center.y + other.halfSize.y);
					}
				}

				return true;
			}

			return false;
		}
	};

	template <typename T> struct Data
	{
		Point pos;
		T box;
		
		Data(Point p = Point(), T data = T())
		{
			pos = p; 
			box = data;
		};
	};

	template <class T> class QuadTree
	{
		const int MAX_OBJECTS = 10;
		const int MAX_LEVELS = 5;

	public:
		QuadTree<T>()
		{
			_northWest = nullptr;
			_northEast = nullptr;
			_southWest = nullptr;
			_southEast = nullptr;
			_boundary = AABB();
			_objects = vector<Data<T>>();
		}

		QuadTree<T>(AABB boundary)
		{
			_northWest = nullptr;
			_northEast = nullptr;
			_southWest = nullptr;
			_southEast = nullptr;
			_boundary = boundary;
			_objects = vector<Data<T>>();
		}

		~QuadTree()
		{
			delete _northWest;
			delete _northEast;
			delete _southWest;
			delete _southEast;
		}

		bool insert(Data<T> d)
		{
			if (!_boundary.contains(d.pos))
			{
				return false;
			}

			if (_objects.size() < MAX_OBJECTS)
			{
				_objects.push_back(d);
				return true;
			}

			if (_northWest == nullptr)
			{
				split();
			}

			if (_northWest->insert(d))
			{
				return true;
			}

			if (_northEast->insert(d))
			{
				return true;
			}

			if (_southWest->insert(d))
			{
				return true;
			}

			if (_southEast->insert(d))
			{
				return true;
			}

			return false;
		}

		void split()
		{
			Point qSize = { _boundary.halfSize.x, _boundary.halfSize.y };
			Point qCenter = { _boundary.center.x - qSize.x, _boundary.center.y - qSize.y };
			_northWest = new QuadTree(AABB(qCenter, qSize));

			qCenter = Point(_boundary.center.x + qSize.x, _boundary.center.y - qSize.y);
			_northEast = new QuadTree(AABB(qCenter, qSize));

			qCenter = Point(_boundary.center.x - qSize.x, _boundary.center.y + qSize.y);
			_southWest = new QuadTree(AABB(qCenter, qSize));

			qCenter = Point(_boundary.center.x + qSize.x, _boundary.center.y + qSize.y);
			_southEast = new QuadTree(AABB(qCenter, qSize));
		}

		vector<Data<T>> queryRange(AABB range)
		{
			vector<Data<T>> pInRange = vector<Data<T>>();
			float scratch = 0, scratch2 = 0;
			if (!_boundary.intersects(range,scratch,scratch2))
			{
				return pInRange;
			}

			for (uint i = 0; i < _objects.size(); i++)
			{
				if (range.contains(_objects.at(i).pos))
				{
					pInRange.push_back(_objects.at(i));
				}
			}

			if (_northWest == nullptr)
			{
				return pInRange;
			}

			vector<Data<T>> temp = _northWest->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = _northEast->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = _southWest->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			temp = _southEast->queryRange(range);
			pInRange.insert(pInRange.end(), temp.begin(), temp.end());

			return pInRange;
		}

	private:

		QuadTree* _northWest;
		QuadTree* _northEast;
		QuadTree* _southWest;
		QuadTree* _southEast;

		AABB _boundary;

		vector<Data<T>> _objects;
	};

}