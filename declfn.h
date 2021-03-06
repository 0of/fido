/*
 * LTest
 *
 * Copyright (c) 2015-2016 "0of" Magnus
 * Licensed under the MIT license.
 * https://github.com/0of/LTest/blob/master/LICENSE
 */
#ifndef DECLFN_H
#define DECLFN_H

#include <type_traits>

namespace LTest {
  namespace Traits {
    template<typename T>
    struct function_trait;

    template<typename Class, typename Return, typename... Arg>
    struct function_trait<Return(Class::*)(Arg...)> {
        using type = std::function<Return(Arg...)>;
    };
    
    template<typename Class, typename Return, typename... Arg>
    struct function_trait<Return(Class::*)(Arg...) const> : public function_trait<Return(Class::*)(Arg...)>{};
    
    template<typename Class, typename Return, typename... Arg>
    struct function_trait<Return(Class::*)(Arg...) volatile> : public function_trait<Return(Class::*)(Arg...)>{};

    template<typename Class, typename Return, typename... Arg>
    struct function_trait<Return(Class::*)(Arg...) const volatile> : public function_trait<Return(Class::*)(Arg...)>{};
    
    template<typename Return, typename... Args>
    struct function_trait<Return(Args...)> {
        using type = std::function<Return(Args...)>;
    };
    
    template<typename Return, typename... Args>
    struct function_trait<Return(Args...) volatile> : public function_trait<Return(Args...)> {};
    
    template<typename T>
    using function_trait_t = typename function_trait<T>::type;
  } // Traits

  struct callable_trait {
    template<typename Callable>
    static auto INVOKE(Callable&& callable, std::true_type isFunction) -> Traits::function_trait_t<std::remove_pointer_t<std::decay_t
    <Callable>>>;
    
    template<typename Callable>
    static auto INVOKE(Callable&& callable, std::false_type isFunction) -> Traits::function_trait_t<decltype(&std::decay_t<Callable>::operator())>;
    
    static auto INVOKE(...) -> std::false_type;
  };

#ifndef declfn
# define declfn(fn) decltype(LTest::callable_trait::INVOKE(fn, std::is_function<std::remove_pointer_t<decltype(fn)>>()))
#endif // declfn
} // LTest

#endif // DECLFN_H
