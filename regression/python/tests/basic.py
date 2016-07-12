import unittest

class Basic(unittest.TestCase):
    def test_import(self):
        try:
            import esbmc
            self.assertTrue(True)
        except:
            self.assertTrue(False, "Failed to import esbmc")

    def test_types(self):
        import esbmc
        # Ensure that the base list of types are in the type class
        base_types = ['array', 'bool', 'code', 'cpp_name', 'empty', 'fixedbv',\
                'pointer', 'signedbv', 'string', 'struct', 'symbol', 'type2t',\
                'type_ids', 'type_vec', 'union', 'unsignedbv']
        for x in base_types:
            self.assertTrue(x in dir(esbmc.type), "Missing type {}".format(x))

    def test_exprs(self):
        import esbmc
        base_exprs = ['abs', 'add', 'address_of', 'and', 'ashr', 'bitand',\
                'bitnand', 'bitnor', 'bitnot', 'bitnxor', 'bitor', 'bitxor',\
                'byte_extract', 'byte_update', 'code_asm', 'code_assign',\
                'code_block', 'code_comma_id', 'code_decl', 'code_expression',\
                'code_free', 'code_function_call', 'code_goto', 'code_init',\
                'code_printf', 'code_return', 'code_skip', 'concat',\
                'constant_array', 'constant_array_of', 'constant_bool',\
                'constant_fixedbv', 'constant_int', 'constant_string',\
                'constant_struct', 'constant_union', 'cpp_catch',\
                'cpp_del_array', 'cpp_delete', 'cpp_throw', 'cpp_throw_decl',\
                'cpp_throw_decl_end', 'deallocated_obj', 'dereference', 'div',\
                'dynamic_object', 'dynamic_size', 'equality', 'expr2t',\
                'expr_ids', 'expr_vec', 'greaterthan', 'greaterthanequal',\
                'if', 'implies', 'index', 'invalid', 'invalid_pointer',\
                'isinf', 'isnan', 'isnormal', 'lessthan', 'lessthanequal',\
                'lshr', 'member', 'modulus', 'mul', 'neg', 'not', 'notequal',\
                'object_descriptor', 'or', 'overflow', 'overflow_cast',\
                'overflow_neg', 'pointer_object', 'pointer_offset',\
                'same_object', 'shl', 'sideeffect', 'sideeffect_allockind',\
                'sub', 'symbol', 'symbol_renaming', 'typecast', 'unknown',\
                'valid_object', 'with', 'xor']
        for x in base_exprs:
            self.assertTrue(x in dir(esbmc.expr), "Missing expr {}".format(x))
