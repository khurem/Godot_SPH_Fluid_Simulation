extends Node

func _on_Area_random_test(node, new_pos):
    print("The position of " + node.name + " is now " + str(new_pos))
