/**
 * @file jalp_structured_data.h This file defines structures and functions to
 * deal with structured_data elements of the syslog metadata.
 *
 * @section LICENSE
 *
 * Source code in 3rd-party is licensed and owned by their respective
 * copyright holders.
 *
 * All other source code is copyright Tresys Technology and licensed as below.
 *
 * Copyright (c) 2011 Tresys Technology LLC, Columbia, Maryland, USA
 *
 * This software was developed by Tresys Technology LLC
 * with U.S. Government sponsorship.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef JALP_STRUCTURED_DATA_H
#define JALP_STRUCTURED_DATA_H
#include <jalop/jalp_param.h>
/**
 * Represents a set of structured data elements. Applications should use an ID
 * containing an '@' unless using one of the registered IDs.
 *
 * Applications must use the jalp_structured_data_create() and
 * jalp_structured_data_destroy() functions to allocate and destroy structured
 * data objects. The jalp_structured_data objects assume ownership of all
 * pointers and will free them using the appropriate "*_destroy()" functions or
 * free().
 *
 * @see rfc5424
 */
struct jalp_structured_data {
	/** The SD-ID for all param elements in \p param_list. */
	char *sd_id;
	/** A list of params belonging to this SD-ID*/
	struct jalp_param *param_list;
	/** The next structured data group */
	struct jalp_structured_data *next;
};
/**
 * Create a jalp_structured_data element.
 *
 * @param[in] prev The location in a list to add the element. If prev is not the
 * end of the list, then the new node is created as the next element of prev,
 * and prev->next becomes the new node's next element.
 * This function may be used to add elements to the end, or middle
 * of a list. #prev may be NULL.
 * @param[in] sd_id The sd_id to use for the new element.
 * @param[in] name The name of this element
 * @param[in] value The value of this element.
 *
 * @return a newly created jalp_structred_data pointer. This must be freed with
 * jalp_structured_data_destroy(struct jalp_structured_data*).
 *
 * sd_id, name, and value are copied to allocated buffers.
 */
struct jalp_structured_data *jalp_structured_data_append(struct jalp_structured_data *prev,
							 char *sd_id);
/**
 * Release all memory associated with this structured data list. This frees all
 * params of #group and all #jalp_structured_data elements.
 * @param[in,out] group The list of SD groups to destroy. This will be set to
 * NULL.
 */
void jalp_structured_data_destroy(struct jalp_structured_data **sd_group);
#endif //JALP_STRUCTURED_DATA_H

