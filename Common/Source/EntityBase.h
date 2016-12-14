#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

#include  <string>
using std::string;

class EntityBase
{
public:
    EntityBase(string name = "");
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
    inline void SetPosition(float x, float y, float z) { position.x = x; position.y = y; position.z = z; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

    bool GetIsInSceneGraph();   // Check if this entity is inside SceneGraph
    void SetInSceneGraph(const bool b_isInGraph);

    string GetName() const;

protected:
	Vector3 position;
	Vector3 scale;

    string name;

	bool isDone;
	bool m_bCollider;
	bool bLaser;

    bool m_bIsInSceneGraph;
};

#endif // ENTITY_BASE_H