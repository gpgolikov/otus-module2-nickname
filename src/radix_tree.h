#pragma once

#include <cassert>
#include <string>
#include <string_view>
#include <map>
#include <forward_list>
#include <algorithm>
#include <utility>

namespace griha {

template <typename CharT>
class radix_tree {

public:
    using string_type = std::basic_string<CharT>;
    using string_view_type = std::basic_string_view<CharT>;

private:
    struct node_type;
    using nodes_type = std::map<CharT, node_type>;
    struct node_type {
        string_type label;
        bool end_flag; // indicates end of word
        nodes_type childs;
    };

    struct iterator_impl {

        using difference_type = void;
        using value_type = std::pair<std::string, std::string>;
        using reference_type = const value_type&;
        using pointer_type = const value_type*;

        const nodes_type& top_nodes;
        std::forward_list<typename nodes_type::const_iterator> path;

        explicit iterator_impl(const nodes_type& tn, typename nodes_type::const_iterator it)
            : top_nodes(tn) {
            path.push_front(it);
        }

        void go_down_at_left() {
            assert(!path.empty());
            assert(path.front() != top_nodes.end());

            const auto& n = path.front()->second;
            if (n.end_flag) {
                return;
            }
            assert(!n.childs.empty());
            path.push_front(n.childs.begin());
            go_down_at_left();
        }

        void go_down_at_right() {
            assert(!path.empty());
            assert(path.front() != top_nodes.end());

            const auto& n = path.front()->second;
            if (n.end_flag) {
                return;
            }
            assert(!n.childs.empty());
            path.push_front(std::prev(n.childs.end()));
            go_down_at_right();
        }

        void next() {
            assert(!path.empty());

            auto it = path.front();
            if (it == top_nodes.end()) {
                return;
            }

            if (it->second.childs.empty()) {
                // go right or go up and right
                path.pop_front();
                for (++it; 
                     it != top_nodes.end() &&
                        it == path.front()->second.childs.end();
                     ++it) {
                    it = path.front();
                    path.pop_front();
                }
                path.push_front(it);
                if (it == top_nodes.end()) {
                    return; // iterator has become end iterator
                }
            } else {
                // go down
                path.push_front(it->second.childs.begin());
            }
            go_down_at_left();
        }

        void prev() {
            assert(!path.empty());
            assert(!top_nodes.empty());

            auto it = path.front();
            if (it->second.childs.empty()) {
                // go left or go up and left
                // if achieved begin of 
                path.pop_front();
                while (it != top_nodes.begin() &&
                        it == path.front()->second.childs.begin()) {
                    it = path.front();
                    path.pop_front();
                }
                if (it != top_nodes.begin()) {
                    --it;
                }
                path.push_front(it);
            } else {
                // go down
                path.push_front(std::prev(it->second.childs.end()));
            }
            go_down_at_right();
        }
    };

public:
    void insert(string_view_type value) {
        if (value.empty()) {
            return;
        }

        insert(nodes[value[0]], value);
    }

private:
    void insert(node_type& n, string_view_type value) {

        if (n.label.empty()) {
            // node 'n' is a place for store 'value' - empty leaf
            n.label = value;
            n.end_flag = true;
            return;
        }

        // find mismatch of two strings
        auto [it_n, it_v] = std::mismatch(
            n.label.begin(), n.label.end(),
            value.begin(), value.end());

        if (it_n != n.label.end() ) {
            // get match prefix and create new node for replacing with node 'n'
            node_type new_n = { string_type{ n.label.begin(), it_n }, false };
            n.label = string_type{ it_n, n.label.end() };
            new_n.childs.emplace(n.label[0], std::move(n));
            n = std::move(new_n);
        }

        if (it_v != value.end()) {
            // insert right part of new value to childs
            insert(n.childs[*it_v], value.substr(it_v - value.begin()));
        } else {
            // new value is prefix of exist word
            // mark it as end of word
            n.end_flag = true;
        }
    }

private:
    nodes_type nodes;
};

} // namespace griha