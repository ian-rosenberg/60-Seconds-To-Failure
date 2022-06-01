#include "contactlistener.h"

ContactListener::ContactListener() {
}

ContactListener::~ContactListener() {
}

void ContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    Contact m_contact = { nullptr, nullptr };
    m_contact.fixtureA = contact->GetFixtureA();
    m_contact.fixtureB = contact->GetFixtureB();
    _contacts.push_back(m_contact);
}

void ContactListener::EndContact(b2Contact* c) {
    Contact contact = { c->GetFixtureA(), c->GetFixtureB() };
    std::vector<Contact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), contact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
}

void ContactListener::PreSolve(b2Contact* contact,
    const b2Manifold* oldManifold) {
}

void ContactListener::PostSolve(b2Contact* contact,
    const b2ContactImpulse* impulse) {
}
