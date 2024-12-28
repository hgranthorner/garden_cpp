#import "document.hpp"

namespace Garden {

using val = emscripten::val;

val Document::query_selector(const std::string &target) {
  return _document.call<val>("querySelector", target);
}

val Document::get_element_by_id(const std::string &id) {
  return _document.call<val>("getElementById", id);
}

val Document::create_element(const std::string &element_type) {
  return _document.call<val>("createElement", element_type);
}

inline static val _document = val::global("document");

} // namespace Garden
