# Dependency Injection

## Introduction
This library is header only. To use it just include di/di.hpp in your project.

Dependency injection is an implementation of Inversion of Control known from java frameworks such as Spring or Qi4j. Still C++ community lacks a dependency injection framework that would be portable and independent from libraries like Qt. This is an attempt to create a simple yet powerful DI library built on boost.

## Injection subject
A class that requires some objects to be injected into it upon instantiation or delegation has to be declared an injection subject. This is achieved by subclassing di::subject class (declared in di/subject.hpp) in the following manner:
```cpp
struct MyClass: public di::subject<T,U,V,...> {};
```
**Note** | ```di::subject<…>``` contains virtual functions therefore inheriting from it causes ```MyClass``` to increase in size by the size of one pointer

For the sake of clarity, let us consider a simple example. Assuming that a Car class requires four wheels, one handbrake and two seats, its declaration would look like this:
```cpp
struct Car: public di::subject<Wheel,Wheel,Wheel,Wheel,HandBrake,Seat,Seat> {};
```

However the order of those parameters is not important. Only the quantity of each type matters. Therefore declaration below is equivalent to the one above.
```cpp
struct Car: public di::subject<Seat,Wheel,Wheel,HandBrake,Wheel,Seat,Wheel> {};
```

## Declaring injections
A class which is an injection subject may declare attributes which will be placeholders for the injections. The types of injections have to match those defined in template parameters of ```di::subject```, though as already mentioned there is no relation between their order. Extending previous example with injection placeholders results in:
```cpp
struct Car: public di::subject<Seat,Wheel,Wheel,HandBrake,Wheel,Seat,Seat,Wheel>
{
    di::required<Wheel> frontLeftWheel;
    di::required<Wheel> frontRightWheel;
    di::required<Wheel> rearLeftWheel;
    di::required<Wheel> rearRightWheel;
    di::required<Seat> leftSeat;
    di::required<Seat> rightSeat;
    di::optional<Seat> backSeat;
    di::required<HandBrake> handBrake;

    Car()
    {
        //injections are not yet available
    }
};
```

```di::required``` class is defined in di/required.hpp and ```di::optional``` class is defined in di/optional.hpp header. Both ```di::required``` and ```di::optional``` have pointer semantics and are convertible to pointer or reference of their template arguments. The injection process is performed right after leaving the constructor in case of ```build()``` method or on demmand in case of ```build(…)``` or ```build_part(…)```. Therefore using any of the required/optional class attributes inside constructor is equivalent to using an uninitialized pointer.

## Types of injections
There are situtations when it is impossible to determine which part of a system is responsible for releasing some object. In such situations it is advised to use smart pointers. DI therefore provides means of handling pointers in different ways. Four distinct types of injections are available. All of them provide pointer semantics.
* di::ordinary
* di::shared
* di::unique
* di::service

### di::ordinary
Indicates that the injection is represented as a bare pointer and the user is responsible for deleting the object.
** Note ** | not providing injection type at all is equivalent to using di::ordinary
```cpp
struct Car : public di::subject<Wheel>
{
    di::required< di::ordinary<Wheel> > wheel;
};
```
is the same as
```cpp
struct Car : public di::subject<Wheel>
{
    di::required<Wheel> wheel;
};
```

### di::shared
Indicates that the injection is represented as a shared_ptr. The object will be released when there are no more associations to it. Due to its internal represantation ```di::shared``` injections can be converted to ```boost::shared_ptr```.
```cpp
struct Car : public di::subject<Wheel>
{
    di::optional< di::shared<Wheel> > wheel;
};
```

### di::unique
Indicates that the injection will be represented as ```unique_ptr``` if available, shared_ptr if ```DI_NO_AUTO_PTR``` is set or ```auto_ptr``` otherwise. Injection subject which requires an injection of type ```di::unique``` will become noncopyable.
```cpp
// Car is noncopyable bacause wheel injection is of type di::unique
struct Car: public di::subject<Wheel>
{
    di::optional< di::unique<Wheel> > wheel;
};
```

### di::service
Is a special kind of injection that is used in conjunction with ```di::module``` and ```di::application```. It is represented by a ```boost::shared_ptr``` and can be converted to it.
```cpp
struct Car : public di::subject<Wheel>
{
    di::optional< di::service<Logger> > logger;
};
```
**Note** | Injection size is equal to the size of its representation

## Builder
The most basic form of building a dependency injection subject is by utilizing ```builder``` defined in di/builder.hpp. ```di::builder``` is a class with two template paramters from which only the first is obligatory as it names the real type ```builder``` is able to build.

Having a ```Car``` class which derives from ```subject``` and a ```builder<Car>``` an instance of Car may be created. However before that could happen, ```builder<Car>``` needs to know the objects it is supposed to inject to ```Car```.
```cpp
di::builder<Car> builder;

Wheel w1,w2,w3,w4;
builder.use(w1).use(w2).use(w3).use(w4);

Seat s1,s2;
HandBrake hb;
builder.use(s1).use(s2).use(hb);

//if available returns unique_ptr otherwise auto_ptr or shared_ptr
unique_ptr<Car> car = builder.build();

assert(&w1 == car->frontLeftWheel);
assert(&w2 == car->frontRightWheel);
assert(&w3 == car->rearLeftWheel);
assert(&w4 == car->rearRightWheel);
assert(&s1 == car->leftSeat);
assert(&s1 == car->rightSeat);
assert( 0  == car->backSeat);
assert(&hb == car->handBrake);
```

In contradiction to subject’s template parameters, the order of use calls is strictly bound to the order in which requireds and optionals are declared. This is bound to the order of requireds and optionals of same types e.g. in the above example changing the order of use calls to ```builder.use(w4).use(w2).use(w3).use(w1)``` would result in ```car->frontLeftWheel == &w4``` and ```car->rearRightWheel == &w1```. Furthermore the injection process is greedy i.e. it injects objects objects to the first field that matches a type. For this reason changing the order in which optionals and requireds are declared would result in an error (assert, exception etc. depending on user’s choice), because the third second seat passed to builder would be injected into ```optional```, thus leaving the last ```required<Seat>``` unsatisfied. By the rule of thumb always put optionals after requireds.
```cpp
struct Car: public di::subject<Seat,Wheel,Wheel,HandBrake,Wheel,Seat,Wheel>
{
    di::required<Wheel> frontLeftWheel;  //<-- first use<Wheel>(...) call
    di::required<Wheel> frontRightWheel; //<-- second use<Wheel>(...) call
    di::required<Wheel> rearLeftWheel;   //<-- third use<Wheel>(...) call
    di::required<Wheel> rearRightWheel;  //<-- fourth use<Wheel>(...) call
    di::required<Seat> leftSeat;         //<-- first use<Seat>(...) call
    di::required<Seat> rightSeat;        //<-- second use<Seat>(...) call
    di::optional<Seat> backSeat;         //<-- third use<Seat>(...) call
    di::required<HandBrake> handBrake;   //<-- first use<HandBrake>(...) call
};
```
```cpp
struct Car: public di::subject<Seat,Wheel,Wheel,HandBrake,Wheel,Seat,Wheel>
{
    di::required<Wheel> frontLeftWheel;  //<-- first use<Wheel>(...) call
    di::required<Wheel> frontRightWheel; //<-- second use<Wheel>(...) call
    di::required<Wheel> rearLeftWheel;   //<-- third use<Wheel>(...) call
    di::required<Wheel> rearRightWheel;  //<-- fourth use<Wheel>(...) call
    di::required<Seat> leftSeat;         //<-- first use<Seat>(...) call
    di::optional<Seat> backSeat;         //<-- second use<Seat>(...) call
    di::required<Seat> rightSeat;        //<-- third use<Seat>(...) call
    //builder knows only about two seats, third seat will cause an error
    di::required<HandBrake> handBrake;   //<-- first use<HandBrake>(...) call
};
```
**Note** | All builders are injection type aware i.e. ```builder.use(di::shared<Wheel>(w1))``` will be injected to ```di::required< di::shared<Wheel> >``` and not to ```di::required<Wheel>```.

## Abstract builder
In many cases it may be impractical to use raw builder<T> due to higher coupling and decreased testability. Prefferably one might want to be able to build an object of class Car while passing a handle to class Vehicle (base of Car). Furthermore it is reasonable to expect to have the possibility of mocking builder::build(T&) method to build/inject mocks/stubs instead of real objects.
Considering:
```cpp
struct Vehicle : public di::subject<Seat,Wheel,Wheel,HandBrake,Wheel,Seat,Wheel>
{
    virtual void move(...) = 0;
};

struct Car: public Vehicle
{
    di::required<Wheel> frontLeftWheel;
    di::required<Wheel> frontRightWheel;
    di::required<Wheel> rearLeftWheel;
    di::required<Wheel> rearRightWheel;
    di::required<Seat> leftSeat;
    di::required<Seat> rightSeat;
    di::required<HandBrake> handBrake;
    virtual void move(...) {...};
};
```
one might write
```cpp
di:abstract_builder<Vehicle>* vehicle_builder = new di::builder<Car,Vehicle>();
Vehicle* porshe = new Porshe();
vehicle_builder->build(*porshe);
```
**Note** | The class which is the template parameter of ```di::abstract_builder``` is the one that needs to derive (publicly) from ```di::subject```. This is a necessary limitation.

## Building families of objects
Aforementioned ```di::abstract_builder::build(T&)``` method allows building families of objects. In the provided example all classes which are subclasses of ```Construct``` are considered to be members of one family and can be build using the same builder. Additionaly this example has been enriched with a code showing a two phased building (trucks are built once for theirs Construct-subjects and once for theirs Vehicle-subjects).
```cpp
class Construct : public di::subject<Logger,Database,ConversionTools> {};

class ConstructImp : public Construct
{
    di::required<Logger> log;
    di::required<Database> db;
    di::required<ConversionTools> conversion;
};

class Truck : public Car, public ConstructImp {};

class Road : public ConstructImp {};

class Garage : public ConstructImp {};


Truck scania;
Truck mercedes;
Road highway;
Garage garage1,garage2;

...

di:abstract_builder<Vehicle>* vehicle_builder = new di::builder<Car,Vehicle>()
di:abstract_builder<Construct>* construct_builder = new di::builder<Construct,ConstructImp>()

//setup both builders

...

vehicle_builder->build(scania);
construct_builder->build(scania);

vehicle_builder->build(mercedes);
construct_builder->build(mercedes);

construct_builder->build(highway);
construct_builder->build(garage1);
construct_builder->build(garage2);
```
