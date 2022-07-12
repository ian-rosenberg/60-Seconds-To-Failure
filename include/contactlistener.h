#pragma once

#include <box2d/box2d.h>
#include <vector>
#include <algorithm>

typedef struct Contact {
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    bool operator==(const Contact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};

class ContactListener : public b2ContactListener {

public:
    std::vector<Contact>_contacts;

    ContactListener();
    ~ContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

};
