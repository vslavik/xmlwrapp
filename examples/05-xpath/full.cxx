#include <xmlwrapp/xmlwrapp.h>
#include <xmlwrapp/xpath.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

template <typename To, typename From> To lexical_cast (const From &from);
void show_user (xpath::const_object::reference user);

int main () {
  try {
    // load full.xml into doc
    xml::document doc;
    xml::tree_parser("full.xml").get_document().swap(doc);

    // show each /root/user
    xpath::const_object users = xpath::query(doc, "/root/user");
    std::for_each(users.begin(), users.end(), show_user);

    // update counter
    xpath::object counter_node = xpath::query(doc, "/root/counter");
    double counter = counter_node.to_number() + 1;
    counter_node->set_content(lexical_cast<std::string>(counter).c_str());
    doc.save_to_file("full.xml");
    std::cout << "COUNTER: " << counter << '\n';
  }

  catch (const std::exception &e) {
    std::cout << "EXCEPTION: " << e.what() << '\n';
  }
}

void show_user (xpath::const_object::reference user_node) {
  xpath::const_context user(user_node);

  std::cout << "        Name: " << user["name"].to_string() << '\n'
	    << "          ID: " << user["id"].to_number() << '\n'
	    << "        Oper? "
	    << (user["oper"].to_boolean() ? "Yes" : "No")
	    << "\n";

  user.set("subs", user["subordinates"]);
  xpath::const_object sub_names = user["../user[id = $subs/id]/name"];

  if (!sub_names.empty()) {
    std::cout << "Subordinates: ";

    for (xpath::const_object::size_type i = 0; i < sub_names.size(); ++i) {
      if (i > 0) std::cout << ", ";
      std::cout << sub_names[i]->get_content();
    }

    std::cout << '\n';
  }

  std::cout << "---\n";;
}

template <typename To, typename From>
To lexical_cast (const From &from) {
  std::stringstream ss;
  if (!(ss << from)) throw std::bad_cast();
  To to;
  ss >> to;
  return to;
}
