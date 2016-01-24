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
