#pragma once

#import <emscripten/val.h>

namespace Garden {

using val = emscripten::val;

class Document {
public:
  static val query_selector(const std::string &target);
  static val get_element_by_id(const std::string &id);
  static val create_element(const std::string &element_type);

private:
  inline static val _document = val::global("document");
};

} // namespace Garden
