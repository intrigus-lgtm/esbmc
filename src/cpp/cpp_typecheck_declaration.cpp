/*******************************************************************\

Module: C++ Language Type Checking

Author: Daniel Kroening, kroening@cs.cmu.edu

\********************************************************************/

#include <cpp/cpp_declarator_converter.h>
#include <cpp/cpp_typecheck.h>
#include <util/expr_util.h>
#include <util/i2string.h>

bool cpp_typecheckt::convert_typedef(typet &type)
{
  if (
    type.id() == "merged_type" && type.subtypes().size() >= 2 &&
    type.subtypes()[0].id() == "typedef")
  {
    type.subtypes().erase(type.subtypes().begin());
    return true;
  }

  return false;
}

void cpp_typecheckt::convert_anonymous_union(
  cpp_declarationt &declaration,
  codet &code)
{
  codet new_code("block");
  new_code.reserve_operands(declaration.declarators().size());

  // unnamed object
  std::string identifier = "#anon_union" + i2string(anon_counter++);

  irept name("name");
  name.identifier(identifier);
  name.set("#location", declaration.location());

  cpp_namet cpp_name;
  cpp_name.move_to_sub(name);
  cpp_declaratort declarator;
  declarator.name() = cpp_name;

  cpp_declarator_convertert cpp_declarator_converter(*this);

  const symbolt &symbol =
    cpp_declarator_converter.convert(declaration, declarator);

  if (!cpp_is_pod(declaration.type()))
  {
    err_location(follow(declaration.type()).location());
    str << "anonymous union is not POD";
    throw 0;
  }

  codet decl_statement("decl");
  decl_statement.reserve_operands(2);
  decl_statement.copy_to_operands(cpp_symbol_expr(symbol));

  new_code.move_to_operands(decl_statement);

  // do scoping
  symbolt &union_symbol = *context.find_symbol(follow(symbol.type).name());
  const irept::subt &components = union_symbol.type.add("components").get_sub();

  forall_irep (it, components)
  {
    if (it->type().id() == "code")
    {
      err_location(union_symbol.type.location());
      str << "anonymous union `" << union_symbol.name
          << "' shall not have function members\n";
      throw 0;
    }

    const irep_idt &base_name = it->base_name();

    if (cpp_scopes.current_scope().contains(base_name))
    {
      err_location(union_symbol.type.location());
      str << "identifier `" << base_name << "' already in scope";
      throw 0;
    }

    cpp_idt &id = cpp_scopes.current_scope().insert(base_name);
    id.id_class = cpp_idt::SYMBOL;
    id.identifier = it->name();
    id.class_identifier = union_symbol.id;
    id.is_member = true;
  }

  union_symbol.type.set("#unnamed_object", symbol.name);

  code.swap(new_code);
}

void cpp_typecheckt::convert(cpp_declarationt &declaration)
{
  // see if the declaration is empty
  if (declaration.find("type").is_nil() && !declaration.has_operands())
    return;

  // Record the function bodies so we can check them later.
  // This function is used recursively, so we save them.
  function_bodiest old_function_bodies = function_bodies;
  function_bodies.clear();

  // templates are done in a dedicated function
  if (declaration.is_template())
    convert_template_declaration(declaration);
  else
    convert_non_template_declaration(declaration);

  typecheck_function_bodies();
  function_bodies = old_function_bodies;
}

void cpp_typecheckt::convert_non_template_declaration(
  cpp_declarationt &declaration)
{
  assert(!declaration.is_template());

  // we first check if this is a typedef
  typet &type = declaration.type();
  bool is_typedef = convert_typedef(type);

  // typedefs are expanded late!

  typecheck_type(type);

  // Special treatment for anonymous unions
  if (
    declaration.declarators().empty() &&
    follow(declaration.type()).get_bool("#is_anonymous"))
  {
    typet final_type = follow(declaration.type());

    if (final_type.id() != "union")
    {
      err_location(final_type.location());
      throw "top-level declaration does not declare anything";
    }

    codet dummy;
    convert_anonymous_union(declaration, dummy);
  }

  // do the declarators (optional)
  Forall_cpp_declarators(it, declaration)
  {
    // copy the declarator (we destroy the original)
    cpp_declaratort declarator = *it;

    cpp_declarator_convertert cpp_declarator_converter(*this);

    cpp_declarator_converter.is_typedef = is_typedef;

    symbolt &symbol = cpp_declarator_converter.convert(
      type, declaration.storage_spec(), declaration.member_spec(), declarator);

    // any template instance to remember?
    if (declaration.find("#template").is_not_nil())
    {
      symbol.type.set("#template", declaration.find("#template"));
      symbol.type.set(
        "#template_arguments", declaration.find("#template_arguments"));
    }

    // replace declarator by symbol expression
    exprt tmp = cpp_symbol_expr(symbol);
    it->swap(tmp);

    // is there a constructor to be called for the declarator?
    if (symbol.lvalue && declarator.init_args().has_operands())
    {
      symbol.value = cpp_constructor(
        symbol.location,
        cpp_symbol_expr(symbol),
        declarator.init_args().operands());
    }
  }
}
