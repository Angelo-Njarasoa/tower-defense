#pragma once

#include <memory>

namespace patterns {

class Product {
public:
    virtual ~Product() = default;
};

class Factory {
public:
    virtual ~Factory() = default;
    virtual std::unique_ptr<Product> create() const = 0;
};

} // namespace patterns
