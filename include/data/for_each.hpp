#ifndef DATA_FOR_EACH_HPP
#define DATA_FOR_EACH_HPP

#include <data/list/for_each.hpp>
#include <data/queue/for_each.hpp>
#include <data/tree/for_each.hpp>
#include <data/map.hpp>
#include <data/meta/which.hpp>
#include <data/meta/equal.hpp>
#include <data/list/linked_list.hpp>
#include <data/map/rb.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/tools/entry_function.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/tree/linked_tree.hpp>

namespace data {
    
    namespace meta {
        
        // we always know how to construct linked lists,
        // so we can always form a list::for_each
        // type using a linked list as the return type. 
        template <typename function, typename input> 
        struct for_each_list {
            using input_element = typename list::is_list<input>::element;
            using output_element = typename std::__invoke_result<function, input_element>::type;
            using output = linked_list<output_element>;
            
            output operator()(const function f, const input l) const {
                return list::for_each<function, input, output>{}(f, l);
            }
        };
        
        template <typename function, typename input> 
        struct for_each_iterable {
            using condition = is_iterable<input>;
            using input_element = typename condition::element;
            using output_element = typename std::__invoke_result<function, input_element>::type;
            using output = linked_list<output_element>;
            using iterator_list = typename condition::iterator_list;
            
            output operator()(const function f, const input i) const {
                return list::for_each<function, iterator_list, output>{}(f, make_iterator_list(i));
            }
        };
        
        template <typename function, typename input> 
        struct for_each_queue {
            using input_element = typename list::is_list<input>::element;
            using output_element = typename std::__invoke_result<function, input_element>::type;
            using output = functional_queue<linked_list<output_element>>;
            
            output operator()(const function f, const input l) const {
                return queue::for_each<function, input, output>{}(f, l);
            }
        };
        
        template <typename function, typename input> 
        struct for_each_tree {
            using input_element = typename tree::definition::buildable<input>::element;
            using output_element = typename std::__invoke_result<function, input_element>::type;
            using output = linked_tree<output_element>;
            
            output operator()(const function f, const input l) const {
                return tree::for_each<function, input, output>{}(f, l);
            }
        };
        
        template <typename function, typename input, typename key, typename value> 
        struct for_each_value {
            using input_element = typename list::is_list<input>::element;
            constexpr static Equal<input_element, map::entry<key, value>> required{};
            using entry_function = entry_function<key, function, value>;
            using output_element = typename std::__invoke_result<entry_function, input_element>::type;
            
            linked_list<output_element> operator()(const function f, const input l) const {
                return for_each_list<entry_function, input>{}(entry_function{f}, l);
            }
        };
        
        // we know how to make rb_maps, so we can
        // do for_each on a map. 
        template <typename function, typename input> 
        struct for_each_map {
            using requirement = map::is_map<input>;
            constexpr static requirement Satisfied{};
            using entry = typename requirement::entry;
            using key = typename requirement::key;
            using value = typename requirement::value;
            using list = typename requirement::list;
            using outer_function = typename Which<for_each_list<entry_function<key, function, value>, list>, for_each_list<value_function<key, function, value>, list>>::result;
            using inner_function = decltype(outer_function::Function);
            using output_value = typename inner_function::output_element::value;
            using output = rb_map<key, output_value>;
            
            output operator()(const function f, const input m) const {
                return {outer_function{}(inner_function{f}, map::entries(m))};
            }
        };
        
        template <typename function, typename input> 
        struct for_each {
            using inner_function = typename Which<
                for_each_queue<function, input>,
                for_each_list<function, input>,
                for_each_map<function, input>, 
                for_each_iterable<function, input>, 
                for_each_tree<function, input>>::result;
            using output = typename inner_function::output;
            
            output operator()(const function f, const input m) const {
                return {inner_function{}(f, m)};
            }
        };
        
    }

    template <typename f, typename d>
    inline typename meta::for_each<f, d>::output for_each(const f fun, const d data) {
        return meta::for_each<f, d>{}(fun, data);
    }
    
    namespace documentation {
        
        template <typename A, typename B, typename f, typename List>
        struct for_each_list :
            public data::function_definition<f, A, B>,
            public list::is_list<List>, 
            public meta::Equal<typename list::is_list<List>::element, A> {
            
            linked_list<B> use_case(f fun, List a) {
                return data::for_each(fun, a);
            }
            
        };
        
        template <typename A, typename B, typename f, typename Queue>
        struct for_each_queue :
            public data::function_definition<f, A, B>,
            public queue::is_queue<Queue>, 
            public meta::Equal<typename queue::is_queue<Queue>::element, A> {
            
            functional_queue<linked_list<B>> use_case(f fun, Queue a) {
                return data::for_each(fun, a);
            }
            
        };
        
        template <typename A, typename B, typename f, typename Key, typename Map>
        struct for_each_map_1 :
            public data::function_definition<f, A, B>,
            public map::is_map<Map>, 
            public meta::Equal<typename map::is_map<Map>::key, Key>, 
            public meta::Equal<typename map::is_map<Map>::value, A> {
            
            rb_map<Key, B> use_case(f fun, Map ka) {
                return data::for_each(fun, ka);
            }
            
        };
        
        template <typename A, typename B, typename f, typename Key, typename Map>
        struct for_each_map_2 :
            public data::function_definition<f, map::entry<Key, A>, B>,
            public map::is_map<Map>, 
            public meta::Equal<typename map::is_map<Map>::key, Key>, 
            public meta::Equal<typename map::is_map<Map>::value, A> {
            
            rb_map<Key, B> use_case(f fun, Map ka) {
                return data::for_each(fun, ka);
            }
            
        };
        
        template <typename A, typename B, typename f, typename T>
        struct for_each_tree :
            public data::function_definition<f, A, B>,
            public tree::definition::buildable<T>, 
            public meta::Equal<typename tree::definition::buildable<T>::element, A> {
            
            linked_tree<B> use_case(f fun, T t) {
                return data::for_each(fun, t);
            }
            
        };
        
        template <typename A, typename B, typename f, typename i>
        struct for_each_iterable :
            public data::function_definition<f, A, B>,
            public is_iterable<i>, 
            public meta::Equal<typename is_iterable<i>::element, A> {
            
            linked_list<B> use_case(f fun, i a) {
                return data::for_each(fun, a);
            }
            
        };
        
    }

}

#endif
