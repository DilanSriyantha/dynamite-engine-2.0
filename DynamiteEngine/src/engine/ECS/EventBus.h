#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <unordered_map>
#include <typeindex>
#include <functional>
#include <vector>

inline size_t getSubscriberId()
{
	static size_t last_id = 0;
	return last_id++;
}

class EventBase {
public:
	virtual ~EventBase() = default;
};

template <typename EventType>
class Event : public EventBase {
public:
	EventType data;
	
	explicit Event(const EventType& data) : data(data) {}
};

struct SubscriptionHandle {
	std::type_index type;
	size_t id;

	SubscriptionHandle() :
		type(typeid(void)), id(std::numeric_limits<size_t>::max()) {}

	SubscriptionHandle(std::type_index type, size_t id) :
		type(type), id(id) {}

	bool operator==(const SubscriptionHandle& other) const noexcept
	{
		return type == other.type && id == other.id;
	}
};

class EventBus {
public:
	template <typename EventType>
	SubscriptionHandle subscribe(std::function<void(const EventType&)> listener)
	{
		size_t id = getSubscriberId();
		auto& listeners = subscribers[typeid(EventType)];

		listeners.emplace_back(id, [listener](const EventBase& e) {
			listener(static_cast<const Event<EventType>&>(e).data);
		});

		return { typeid(EventType), id };
	}

	void unsubscribe(const SubscriptionHandle& handle)
	{
		std::cout << "sub id: " << handle.id << " request to unsubsubscribe\n";

		auto it = subscribers.find(handle.type);
		if (it == subscribers.end()) return;

		auto& vec = it->second;
		vec.erase(
			std::remove_if(
				vec.begin(),
				vec.end(),
				[&](auto& pair) { return pair.first == handle.id; }
			),
			vec.end()
		);

		// if vector is empty after unsubscription, 
		// clean it
		if (vec.empty())
			subscribers.erase(it);
	}

	template <typename EventType>
	void emit(const EventType& e)
	{
		auto it = subscribers.find(typeid(EventType));
		if (it != subscribers.end())
		{
			for (auto& pair : it->second)
				pair.second(Event<EventType>(e));
		}
	}

	template <typename EventType>
	int getSize()
	{
		auto& listeners = subscribers[typeid(EventType)];
		return listeners.size();
	}

	void clear()
	{
		subscribers.clear();
	}

private:
	std::unordered_map<
		std::type_index, 
		std::vector<std::pair<size_t, std::function<void(const EventBase&)>>>
	> subscribers;
};

#endif