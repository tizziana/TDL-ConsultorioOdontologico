#include "strutil.h"
#include <string.h>
#include <stdlib.h>


char *substr(const char *str, size_t n){
	char* new_str = malloc((n+1)*sizeof(char));
	if (!new_str) return NULL;
	
	char actual;
	int i;
	for (i=0; i<n; i++){
		actual = str[i];
		if (actual == '\0') break;
		new_str[i] = actual;
	}
	new_str[i] = '\0';
	return new_str;
	// no uso snprintf() por ser O(N);
}

char **split(const char *str, char sep){
	size_t len = strlen(str);
	size_t count = 0;
	for (int i=0; i<len; i++){
		if (str[i] == sep) count++;
	}
	
	char **splitted = malloc(sizeof(char*)*(count+2));
	if (!splitted) return NULL;
	count = 0;
	int last_spl = 0;
	for (int i=0; i<len; i++){
		if (str[i] == sep){
			splitted[count] = substr(str+last_spl, i-last_spl);
			count++;
			last_spl = i+1;
		}
	}
	splitted[count] = substr(str+last_spl, len-last_spl);
	splitted[count+1] = NULL;
	return splitted;
}

char *join(char **strv, char sep){
	if (!strv){
		char* joined = malloc(2*sizeof(char));
		joined[0] = '\0';
		return joined;
	}
	size_t len = 0;
	int i = 0;
	char* act = strv[i];
	while (act != NULL){
		len += strlen(act) + 1;
		i++;
		act = strv[i];
	}
	char* joined = malloc((len+1)*sizeof(char));
	if (!joined) return NULL;
	
	i = 0;
	size_t j = 0;
	act = strv[i];
	while (act != NULL){
		size_t len_act = strlen(act);
		for (int k = 0; k<len_act; k++){
			joined[j] = act[k];
			j++;	
		}
		if (sep != '\0'){
			joined[j] = sep;
			j++;
		}
		i++;
		act = strv[i];
	}
	if (sep != '\0' && j != 0) j--;
	// el algoritmo siempre pondra un separador de mas, entonces para finalizar la cadena:
	joined[j] = '\0';
	return joined;
}


void free_strv(char *strv[]){
	if (!strv) return;
	int i = 0;
	char* act = strv[i];
	while (act != NULL){
		free(act);
		i++;
		act = strv[i];
	}
	free(strv);
}

