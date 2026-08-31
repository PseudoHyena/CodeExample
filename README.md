# Messaging System

This plugin was created in response to what I consider an anti-pattern feature from a commercial project I previously worked on.

## The Problem

The game consists of many independent features and systems that need to communicate with each other, even when they are not immediately available.

### Their Solution
They created a system where you can request a so-called dependency object, which the system provides either immediately or once it becomes available. For example, if System A requires data from the `Player State`, you register the `Player State` (Character and Controller) as a dependency within the system under a specific context. System A can then request the `Player State` as a dependency directly. 

At first glance, this system seems convenient, but it is a blatant anti-pattern that caused a multitude of bugs:
* **Black Box Context Resolution:** The system converted contexts based on arbitrary logic (e.g., converting `Player State` to `Player Character` to `Player Controller` and vice versa). This led to bugs where changing the registration of one dependency would break something completely unrelated.
* **Massive Dependency Web:** Instead of following the SOLID principle of dependency inversion, it made systems directly dependent on each other (e.g., System A requesting an object from System B makes it tightly coupled to System B).

## My Solution

To solve this, we first need to deconstruct the actual problem. What kinds of communication are possible between systems?
1. System A wants to subscribe to events from System B.
2. System A wants to make System B execute a task.
3. System A wants to retrieve data from System B.

Instead of acquiring the actual system object and interacting with it directly, we should decouple these three actions into distinct **Messages**. This is where the Messaging System comes into play. 

* The first case is solved using **Notification Messages**.
* The second case is solved using **Command Messages**.
* The third case is solved using **Request/Response Messages**.

### What is a Message?
A Message consists of a key (`FGameplayTag`) and a payload (`FInstancedStruct`). You register the Message within the registry by providing these two elements, after which the system can accept it. 

To handle a specific Message:
1. Your class must inherit the `Handler` interface.
2. The object must be registered within the system as a Handler for the Message (e.g., `"Some.Message.Id"`) while specifying its context (scope).
3. The sender only needs to call the appropriate sending function based on the Message type, passing the specified Context, Message Id, and payload if required.

---

## Message Types & Behaviors

| Type | Relationship | Behavior |
| :--- | :--- | :--- |
| **Notification** | Many-to-Many | One Message can have multiple Senders and Handlers. Notifications are sent immediately without any pending mechanisms. |
| **Command** | Many-to-One | Includes pending and conflict-resolution mechanisms. If a handler does not exist when the Message is sent, the Handler will still receive the pending command upon registration. |
| **Request/Response** | Many-to-One | Similar to Commands, but designed to retrieve an asynchronous (not multithreaded) response. The Message handler is responsible for returning the requested data. |

---

## The Role of Context

Context acts as a scope for the Message within the `UWorld` Outer hierarchy. The root is `UWorld` itself, and using `UWorld` as a context is referred to as using the **Global Context**. 

### Examples:
* **Local Scope:** Suppose we have two components on a Character from two different systems that need to communicate. They can send and handle Messages using their Owner (`AActor`) as the context. That way, a message from Component A of Actor `"Foo"` will not reach Component B of Actor `"Bar"`.
* **Global Scope:** If there is a bomb on the level that players must plant or defuse, the `GameMode` or `GameState` needs to be notified when it explodes. In this scenario, the bomb can send a global Notification Message that the `GameMode` or `GameState` can handle.

## Benefits

What this system achieves:
* **Decoupling:** Different game features can remain independent while remaining fully capable of communicating with each other.
* **Scalability:** The architecture becomes much more modular, making it easier to add, remove, or modify features without breaking unrelated systems.
