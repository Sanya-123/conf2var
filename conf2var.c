#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libconfig.h"
#include "conf2var.h"

void write_node_config(varloc_node_t* node, config_setting_t* cfg){
    if (node == NULL){
        return;
    }
    config_setting_t* group = config_setting_add(cfg, node->name, CONFIG_TYPE_GROUP);
    if(group != NULL){
        config_setting_t* name = config_setting_add(group, "name", CONFIG_TYPE_STRING);
        config_setting_set_string(name, node->name);
        config_setting_t* type = config_setting_add(group, "ctype", CONFIG_TYPE_STRING);
        config_setting_set_string(type, node->ctype_name);
        config_setting_t* var = config_setting_add(group, "nodetype", CONFIG_TYPE_INT);
        config_setting_set_int(var, node->var_type);
        config_setting_t* address = config_setting_add(group, "address", CONFIG_TYPE_INT);
        config_setting_set_int(address, node->address.base);
        config_setting_t* size = config_setting_add(group, "size", CONFIG_TYPE_INT);
        config_setting_set_int(size, node->address.size_bits);
        config_setting_t* offset = config_setting_add(group, "offset", CONFIG_TYPE_INT);
        config_setting_set_int(offset, node->address.offset_bits);
        config_setting_t* sign = config_setting_add(group, "sign", CONFIG_TYPE_INT);
        if (node->is_signed){
            config_setting_set_int(sign, VARLOC_SIGNED);
        }
        else if (node->is_float){
            config_setting_set_int(sign, VARLOC_FLOAT);
        }
        else{
            config_setting_set_int(sign, VARLOC_UNSIGNED);
        }
    }
    else{
        printf("Group null for node: %s\n", node->name);
    }

    if (node->child != NULL){
        config_setting_t* list = config_setting_add(group, "members", CONFIG_TYPE_LIST);
        if (list == NULL && cfg != NULL){
            printf("Array null for node: %s %s\n", node->name, cfg->name);
        }
        write_node_config(node->child, list);
    }
    if (node->next != NULL){
        write_node_config(node->next, cfg);
    }
}

varloc_node_t* read_node_config(config_setting_t* cfg){
    if (cfg == NULL){
        return NULL;
    }
    int count = config_setting_length(cfg);
    varloc_node_t* ret = NULL;
    varloc_node_t* node = NULL;
    for(int i = 0; i < count; ++i){
        if (ret != NULL){
            // sibling
            varloc_node_t* sibling = new_var_node();
            node->next = sibling;
            sibling->previous = node;
            node = sibling;
        }
        else{
            // first node on level
            node = new_var_node();
            ret = node;
        }
        config_setting_t *var = config_setting_get_elem(cfg, i);
        const char* name;
        const char* ctype_name;
        if(config_setting_lookup_string(var, "name", &name)){
            strcpy(node->name, name);
        }
        if(config_setting_lookup_string(var, "ctype", &ctype_name)){
            strcpy(node->ctype_name, ctype_name);
        }
        int sign = 0;
        config_setting_lookup_int(var, "nodetype", &node->var_type);
        config_setting_lookup_int(var, "address", &node->address.base);
        config_setting_lookup_int(var, "size", &node->address.size_bits);
        config_setting_lookup_int(var, "offset", &node->address.offset_bits);
        config_setting_lookup_int(var, "sign", &sign);
        if (sign == VARLOC_SIGNED){node->is_signed = 1;}
        if (sign == VARLOC_FLOAT){node->is_float = 1;}

        config_setting_t* members = config_lookup(&var, "members");
        if(members != NULL){
            node->child = read_node_config(members);
            if (node->child){
                node->child->parent = node;
            }
        }
    }
    return ret;
}

int var2conf(varloc_node_t* root_node, char* conf_path){
    config_t cfg;
    config_init(&cfg);
    config_setting_t* root_setting = config_root_setting(&cfg);

    config_setting_t* vars_list = config_setting_add(root_setting, "variables", CONFIG_TYPE_LIST);
    write_node_config(root_node, vars_list);

    /* Write out the new configuration. */
    if(! config_write_file(&cfg, conf_path)){
        fprintf(stderr, "Error while writing file.\n");
        config_destroy(&cfg);
        return(EXIT_FAILURE);
    }

    fprintf(stdout, "New configuration successfully written to: %s\n",
            conf_path);

    config_destroy(&cfg);

    return(EXIT_SUCCESS);
}


varloc_node_t* conf2var(char* conf_path){
    config_t cfg;
    config_setting_t *vars;
    config_init(&cfg);
    varloc_node_t* ret = NULL;
    if(! config_read_file(&cfg, conf_path)){
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return NULL;
    }

    vars = config_lookup(&cfg, "variables");
    if(vars != NULL){
        ret = read_node_config(vars);
        if(ret == NULL){
            fprintf(stderr, "'variables' field found. No variables decoded\n");
        }
    }
    else{
        fprintf(stderr, "No 'variables' field found in configuration file.\n");
    }
    return ret;

}

