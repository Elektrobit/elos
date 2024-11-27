# API concept to use elos with a C++ interface

## Problem

The libelos-cpp should deliver a smooth and intuitive user experience for using elos with a C++ interface. On one hand, the API’s public functions should be easy to understand and use, with special attention given to transparent and user-friendly error handling. On the other hand, consistency is essential; the API should maintain a stable interface to prevent disruptions for users when updates occur. This means ensuring continuity in public header files, error handling, and any externally observable behaviors, so that users can rely on the API’s stability and won’t need to make frequent adjustments to their code.

## Influencing factors

The following constraints have to be taken into account for a suitable solution:
* elos/safu linker dependencies should be decreased to a minimum
* libelos C-functions must be wrapped in C++ API
* API should be easy to use
* maintain API Stability across versions

## Considerations

# 1) Connect in constructor vs connect is used in each API function

# 1.1) Connect in constructor
If connect is called on construction,we have to deal with error handling in the constructor, which makes the code for the caller complicated:

try {
Elos elos("tcp://localhost:78934");
} catch(...){
...
}

*pros*
* Less user management
*cons*
* Error handling is tricky since constructors can not return error codes. Throwing exceptions in a constructor is possible, but it complicates instance creation and makes it harder to manage resources, especially if exceptions need to be caught and handled gracefully.

# 1.2) connect is used in each API function

auto* elos = new Elos("tcp://localhost:78934");

// optional
elos->connect();

elos->publish();

// optional
elos.disconnect();

delete elos;

connect and disconnect are optional as

connect is called if needed in each API function

publish(*event) {
  if(connected == false) {
    connect();
  }
  // libelos function
  elos_publish(event);
}

and disconnect is called if needed in destructor

Elos::~Elos() {
    if (connected == true) {
        disconnect();
    }
}

*pros*
* Less user management: user does not have to care
  about Connect and Disconnect
* user can have control about connection/disconnection if wanted
  by calling them explicit.
* Error handling can managed by return values on API functions.

# 2) user autonomy vs subscription management

# 2.1) user autonomy

auto* elos = new Elos("tcp://localhost:78934");

auto subscription = elos.subscribe("pattern");

elos.publish(event);

auto events = elos.read(subscription);

for (const auto& event : *events) {
   // do something on event that matches patterns
}

*pros*
* user can implement management as desired.
*cons*
* every user has to implement own management
  which could lead to redundancy and misuse.

# 2.2) subscription management

// new class EventMgt

auto* elos = new Elos("tcp://localhost:78934");
auto* event_mgt = new EventMgt(elos);

// loop through elements to get all events from each subscribed queue
event_mgt.readall();
// add default action if a matched event occur
event_mgt.add_action();
// read events and execute default actions
event_mgt.run();

// or extent class subscription in elos

auto* elos = new Elos("tcp://localhost:78934");

elos.subscription.readall();

elos.subscription.add_action(id);

elos.subscription.run();

*pros*
* user can still implement management as desired.
* additional typical management task are supported.
*cons*
* more development work.

## Decision

Connection is used in each API function since it provides more comfort and flexibility for the user:
Connection is handled in background but can also be called explicitly.
Error handling can be handled on return values on API functions.

All user should given the possibility to run their own management. Elos API function should stay public and not be hidden in management functionality.
If management functions are implemented they should be part of the elos subclass subscription or as a separated class that gets elos instance as reference.

### Open Points

Since API should have a minimum of dependencies to safu/elos, there are some open points:
Error handling can be tricky: Return values (enums) of libelos and libelos-cpp API should be in sync but can not be shared through public headers.
Additionally, struct definitions from libelos cannot be included in the public libelos-cpp headers. The PImpl idiom in C++ (https://en.cppreference.com/w/cpp/language/pimpl) allows for hiding these details by using opaque pointers."

Future decisions on this matters should be documented here.
