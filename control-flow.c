#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Struct to store the assembly instructions and labels
typedef struct {
    char **instructions;
    int *labels;
    int instruction_count;
    int label_count;
} AssemblyData;

// Function to read the assembly file and parse it
AssemblyData* parse_assembly(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    AssemblyData *data = malloc(sizeof(AssemblyData));
    data->instructions = malloc(sizeof(char*) * 1000);
    data->labels = malloc(sizeof(int) * 1000);
    data->instruction_count = 0;
    data->label_count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Strip newline character
        line[strcspn(line, "\n")] = 0;

        // Detect labels (lines ending with ":")
        if (strchr(line, ':') != NULL) {
            data->labels[data->label_count++] = data->instruction_count;
        }

        // Save instruction
        data->instructions[data->instruction_count] = strdup(line);
        data->instruction_count++;
    }

    fclose(file);
    return data;
}

// Function to analyze control flow
void analyze_control_flow(AssemblyData *data) {
    printf("\nControl Flow Analysis Report:\n");

    // Unconditional jumps
    printf("\nUnconditional Jumps:\n");
    for (int i = 0; i < data->instruction_count; i++) {
        if (strncmp(data->instructions[i], "jmp", 3) == 0) {
            printf("Line %d: %s\n", i + 1, data->instructions[i]);
        }
    }

    // Conditional jumps
    printf("\nConditional Jumps:\n");
    for (int i = 0; i < data->instruction_count; i++) {
        if (strncmp(data->instructions[i], "je", 2) == 0 || 
            strncmp(data->instructions[i], "jne", 3) == 0 || 
            strncmp(data->instructions[i], "jg", 2) == 0 ||
            strncmp(data->instructions[i], "jl", 2) == 0 ||
            strncmp(data->instructions[i], "jge", 3) == 0 ||
            strncmp(data->instructions[i], "jle", 3) == 0 ||
            strncmp(data->instructions[i], "jb", 2) == 0 ||
            strncmp(data->instructions[i], "ja", 2) == 0 ||
            strncmp(data->instructions[i], "call", 4) == 0) {
            printf("Line %d: %s\n", i + 1, data->instructions[i]);
        }
    }

    // Function calls
    printf("\nFunction Calls:\n");
    for (int i = 0; i < data->instruction_count; i++) {
        if (strncmp(data->instructions[i], "call", 4) == 0) {
            printf("Line %d: %s\n", i + 1, data->instructions[i]);
        }
    }

    // Function returns
    printf("\nFunction Returns:\n");
    for (int i = 0; i < data->instruction_count; i++) {
        if (strncmp(data->instructions[i], "ret", 3) == 0) {
            printf("Line %d: %s\n", i + 1, data->instructions[i]);
        }
    }

    // Loops (Detect jumps to labels)
    printf("\nLoops (Jumps with associated labels):\n");
    for (int i = 0; i < data->instruction_count; i++) {
        if (strncmp(data->instructions[i], "jmp", 3) == 0 || 
            strncmp(data->instructions[i], "je", 2) == 0 || 
            strncmp(data->instructions[i], "jne", 3) == 0 ||
            strncmp(data->instructions[i], "jg", 2) == 0 ||
            strncmp(data->instructions[i], "jl", 2) == 0) {
            
            // Check if there is a corresponding label that indicates a loop
            for (int j = 0; j < data->label_count; j++) {
                if (strstr(data->instructions[i], data->instructions[data->labels[j]]) != NULL) {
                    printf("Line %d: %s (Looping to label %s)\n", i + 1, data->instructions[i], data->instructions[data->labels[j]]);
                }
            }
        }
    }
}

// Main function to execute the tool
int main() {
    // Ask the user for the assembly file path
    char file_path[MAX_LINE_LENGTH];
    
    printf("=========================Binary Analysis Tool=========================\n\n");
    printf("Enter the path of the assembly file: ");
    fgets(file_path, MAX_LINE_LENGTH, stdin);
    file_path[strcspn(file_path, "\n")] = 0;  // Remove the newline character

    // Parse the assembly file
    AssemblyData *data = parse_assembly(file_path);
    if (data == NULL) {
        return 1;
    }

    // Analyze the control flow
    analyze_control_flow(data);

    // Free allocated memory
    for (int i = 0; i < data->instruction_count; i++) {
        free(data->instructions[i]);
    }
    free(data->instructions);
    free(data->labels);
    free(data);

    return 0;
}
