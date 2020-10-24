# Keep the function signature,
# but replace its body with your implementation.
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions outside of this one,
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions.
import os
import sys
import zlib

def topo_order_commits(dir_to_print="."):
        git_dir = find_git(dir_to_print) + "/.git/"
        branch_dict = get_branches(git_dir)
        branch_dict_keys = list(branch_dict.keys())
        commit_nodes, root_hashes = build_commit_graph(git_dir, branch_dict_keys)
        order = get_topo_ordered_commits(commit_nodes, root_hashes)
        print_topo_ordered_commits_with_branch_names(commit_nodes, order, branch_dict)

def find_git(child_dir):
        var = 1
        while(var == 1): #infinite loop
                if (os.path.abspath(child_dir) == "/"):
                        sys.stderr.write("Not inside a git repository")
                        sys.exit(1)
                for entry in os.listdir(child_dir):
                        if os.path.isdir(os.path.join(child_dir,entry)) and entry == ".git":
                                return os.path.abspath(child_dir)
                child_dir = "../" + child_dir

def get_branches(git_dir):
        if git_dir[-5:] == ".git/":
                heads_dir = git_dir + "refs/heads/"
        else:
                heads_dir = git_dir
        branch_dict = {}
        if os.path.isdir(heads_dir):
                for entry in os.listdir(heads_dir):
                        if (os.path.isdir(heads_dir + entry)):
                                sub_dict = get_branches(heads_dir + entry + "/")
                                branch_dict.update(sub_dict)
                        else:
                                branch_id = open(os.path.join(heads_dir,entry), 'r').read().strip()
                                appendMe = entry
                                if heads_dir[-16:] != ".git/refs/heads/":
                                        appendMe = heads_dir[heads_dir.find(".git/refs/heads/")+16:] + entry
                                if branch_id in branch_dict:
                                        branch_dict[branch_id].append(appendMe)
                                else:
                                        branch_dict[branch_id] = []
                                        branch_dict[branch_id].append(appendMe)
        return branch_dict

class CommitNode:
        def __init__(self, commit_hash):
                """
                :type commit_hash: str
                """
                self.commit_hash = commit_hash
                self.parents = set()
                self.children = set()

def build_commit_graph(git_dir, local_branch_heads):
        root_hashes = set()
        visited = set()
        commit_nodes = {} # dictionary mapping commit_hash to CommitNode
        stack = local_branch_heads
        while len(stack) > 0:
                commit_hash = stack.pop()
                if commit_hash in visited:
                        continue
                visited.add(commit_hash)
                if commit_hash not in commit_nodes:
                        commit_node = CommitNode(commit_hash)
                        commit_nodes[commit_hash] = commit_node
                commit_node = commit_nodes[commit_hash]
                filename = git_dir + "objects/" + commit_hash[:2] + "/" + commit_hash[2:]
                compressed_contents = open(filename, 'rb').read()
                decompressed_contents = zlib.decompress(compressed_contents).decode()
                split_list = decompressed_contents.split("\n")
                for i in split_list:
                        if i[:6] == "parent":
                                pair = i.split()
                                commit_node.parents.add(pair[1])
                if not commit_node.parents:
                        root_hashes.add(commit_hash)
                for i in commit_node.parents:
                        if i not in visited:
                                stack.append(i)
                        if i not in commit_nodes:
                                commit_nodes[i] = CommitNode(i)
                        commit_nodes[i].children.add(commit_hash)
        return commit_nodes, root_hashes

def get_topo_ordered_commits(commit_nodes, root_hashes):
        order = []
        visited = set()
        temp_stack = []
        stack = sorted(root_hashes)
        while stack:
                v = stack.pop()
                if v in visited:
                        continue
                visited.add(v)
                while len(temp_stack) > 0 and v not in commit_nodes[temp_stack[-1]].children:
                        g = temp_stack.pop()
                        order.append(g)
                temp_stack.append(v)
                for c in sorted(commit_nodes[v].children):
                        if c in visited:
                                continue
                        stack.append(c)
        while temp_stack:
                g = temp_stack.pop()
                order.append(g)
        return order

def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
        jumped = False
        for i in range(len(topo_ordered_commits)):
                commit_hash = topo_ordered_commits[i]
                if jumped:
                        jumped = False
                        sticky_hash = ' '.join(commit_nodes[commit_hash].children)
                        print(f'={sticky_hash}')
                branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
                print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
                if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
                        jumped = True
                        sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
                        print(f'{sticky_hash}=\n')

if __name__ == '__main__':
    topo_order_commits(".")
