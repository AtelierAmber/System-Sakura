#include "Bounds.h"

#define collision_displacement 0.005f

namespace Sakura{

	bool Rect::calculateRectangleCollision(Rect& ref){
		if (this->intersect(ref)){
			float x1Intersection = ref.x2 - this->x1;
			float x2Intersection = this->x2 - ref.x1;
			float y1Intersection = this->y1 - ref.y2;
			float y2Intersection = ref.y1 - this->y2;

			float currentMin = x1Intersection;

			if (x2Intersection < currentMin){
				currentMin = x2Intersection;
			}
			if (y1Intersection < currentMin){
				currentMin = y1Intersection;
			}
			if (y2Intersection < currentMin){
				currentMin = y2Intersection;
			}

			if (currentMin == x1Intersection){
				if (isStationary){
					ref.move(-x1Intersection - collision_displacement, 0.0f);
				}
				else if (ref.isStationary){
					this->move(x1Intersection + collision_displacement, 0.0f);
				} /* Else do nothing with collision because both are stationary */
			}
			else if (currentMin == x2Intersection){
				if (isStationary){
					ref.move(x2Intersection + collision_displacement, 0.0f);
				}
				else if (ref.isStationary){
					this->move(-x2Intersection - collision_displacement, 0.0f);
				} /* Else do nothing with collision because both are stationary */
			}
			else if (currentMin == y1Intersection){
				if (isStationary){
					ref.move(0.0f, y1Intersection + collision_displacement);
				}
				else if(ref.isStationary){
					this->move(0.0f, -y1Intersection - collision_displacement);
				} /* Else do nothing with collision because both are stationary */
			}
			else{
				if (isStationary){
					ref.move(0.0f, -y2Intersection - collision_displacement);
				}
				else if (ref.isStationary){
					this->move(0.0f, y2Intersection + collision_displacement);
				} /* Else do nothing with collision because both are stationary */
			}
			return true;
		}
		else return false;
	}

}