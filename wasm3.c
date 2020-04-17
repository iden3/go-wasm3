#include "_cgo_export.h"

// module_get_function is a helper function for the module Go struct
IM3Function module_get_function(IM3Module i_module, int index) {
	IM3Function f = & i_module->functions [index];
	return f;
}

// module_get_imported_function is a helper function for the module Go struct
IM3Function module_get_imported_function(IM3Module i_module, int index) {
    if(i_module==NULL)
        return NULL;
    
    if(i_module->imports == NULL)
        return NULL;

	return i_module->imports[index];
}

int findFunction(IM3Function * o_function, IM3Runtime i_runtime, const char * const i_moduleName, const char * const i_functionName)
{
    void * r = NULL;
    *o_function = NULL;

    IM3Module i_module = i_runtime->modules;

    while (i_module)
    {
        if (i_module->name)
        {
            if (strcmp (i_module->name, i_moduleName) == 0)
            {
                for (u32 i = 0; i < i_module->numFunctions; ++i)
                {
                    IM3Function f = & i_module->functions [i];

                    if (f->name)
                    {
                        if (strcmp (f->name, i_functionName) == 0)
                        {
                            *o_function = f;
                            return 0;
                        }
                    }
                }
            }
        }
        
        i_module = i_module->next;
    }

    return -1;
}

const char* function_get_import_module(IM3Function i_function) {
    return i_function->import.moduleUtf8;
}

const char* function_get_import_field(IM3Function i_function) {
    return i_function->import.fieldUtf8;
}

u8 function_get_arg_type(IM3Function i_function, int index) {
    return i_function->funcType->argTypes[index];
}

int call(IM3Function i_function, uint32_t i_argc, void * i_argv, void * o_result) {
	IM3Module module = i_function->module;
	IM3Runtime runtime = module->runtime;
	m3stack_t stack = (m3stack_t)(runtime->stack);
	IM3FuncType ftype = i_function->funcType;
	if (i_argc != ftype->numArgs) {
		set_error("invalid number of arguments");
		return -1;
	}
	for (int i = 0; i < ftype->numArgs; i++) {
		m3stack_t s = &stack[i];
		switch (ftype->argTypes[i]) {
		case c_m3Type_i32:
		    *(i32*)(s) = (i32)(((u64*)i_argv)[i]);
		    break;
		case c_m3Type_i64:
		    *(i64*)(s) = (i64)(((u64*)i_argv)[i]);
		    break;
		case c_m3Type_f32:
		    *(f32*)(s) = (f32)(((u64*)i_argv)[i]);
		    break;
		case c_m3Type_f64:
		    *(f64*)(s) = (f64)(((u64*)i_argv)[i]);
		    break;
		default:
		    set_error("invalid arg type in function signature");
		    return -1;
		}
	}
	m3StackCheckInit();
	M3Result call_result = Call(i_function->compiled, stack, runtime->memory.mallocated, d_m3OpDefaultArgs);
	if(call_result != m3Err_none) {
		set_error(call_result);
		return -1;
	}
	switch (ftype->returnType) {
		case c_m3Type_i32:
			*(i32*)o_result = (i32)(stack[0]);
			break;
		case c_m3Type_i64:
			*(i64*)o_result = (i64)(stack[0]);
			break;
		case c_m3Type_f32:
			*(f32*)o_result = (f32)(stack[0]);
			break;
		case c_m3Type_f64:
			*(f64*)o_result = (f64)(stack[0]);
			break;
		case c_m3Type_none:
			break;
		default:
			set_error("unexpected return type");
			return -1;
	};
	return 0;
}

int get_allocated_memory_length(IM3Runtime i_runtime) {
	return i_runtime->memory.mallocated->length;
}

u8* get_allocated_memory(IM3Runtime i_runtime) {
	return m3MemData(i_runtime->memory.mallocated);
}

const void * native_dynamicFunctionWrapper(IM3Runtime runtime, uint64_t * _sp, void * _mem, void * cookie) {
    int code = dynamicFunctionWrapper(runtime, _sp, _mem, (uint64_t) cookie);
    return code == 0 ? m3Err_none : m3Err_trapExit;
}

uint64_t nextSlot = 0;
int attachFunction(IM3Module i_module, char* moduleName, char* functionName, char* signature) {
    uint64_t slot = nextSlot++;
    m3_LinkRawFunctionEx(i_module, moduleName, functionName, signature, native_dynamicFunctionWrapper, (void *) slot);
    return slot;
}

