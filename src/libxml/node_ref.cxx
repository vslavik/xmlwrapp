#include "xmlwrapp/node_ref.h"
#include "xpath_helper.h"

xpath::node_ref::node_ref () {
}

xpath::node_ref::node_ref (const node_ref &rhs)
  : xml::node() {
  if (xmlNodePtr node = xpath_helper::get(rhs))
    xpath_helper::set(*this, node);
}

xpath::node_ref &
xpath::node_ref::operator= (const node_ref &rhs) {
  xpath_helper::set(*this, xpath_helper::get(rhs));
  return *this;
}

xpath::node_ref *
xpath::node_ref::operator-> () {
  return this;
}

const xpath::node_ref *
xpath::node_ref::operator-> () const {
  return this;
}

xpath::node_ref &
xpath::node_ref::as_param () {
  return *this;
}

const xpath::node_ref &
xpath::node_ref::as_param () const {
  return *this;
}
