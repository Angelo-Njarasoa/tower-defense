#pragma once

namespace patterns {

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update() = 0;
};

} // namespace patterns
