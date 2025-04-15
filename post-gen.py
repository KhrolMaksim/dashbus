import re

def modify_cpp_file(input_file, output_file):
    with open(input_file, 'r') as file:
        content = file.read()

    # Modify all class declarations to inherit from ParameterPack
    content = re.sub(r'class\s+(\w+)\s*{', r'class \1 : public ParameterPack {', content)

    # Modify private member variables to use SettingsParameter for all types including std::optional
    # Exclude lines with 'return'
    # Match lines that are not return statements
    content = re.sub(r'^(?!.*\breturn\b)(\s*(?:std::optional<[^>]+>|(?:[a-zA-Z_]\w*::)?[a-zA-Z_]\w*)\s+)(\w+);', r'SettingsParameter<\1> \2;', content, flags=re.MULTILINE)

    # Save the modified content to the output file
    with open(output_file, 'w') as file:
        file.write(content)

# Example usage
input_cpp_file = 'Untitled.cpp'  # Replace with your input file name
output_cpp_file = 'Untitled.cpp'  # Use a different name for output to avoid overwriting
modify_cpp_file(input_cpp_file, output_cpp_file)

print("File has been modified successfully!")