/*
 *                  binary position:
 *                  9876543210
 * symbol payloads: 
 *
 *        cons cell xxxxxxxxx0 both car and cdr point to other cells
 *          symbols 0000000011 only symbols have bit 1 (zero-indexed) set
 *           string 0000000101
 *           number 0000001101 
 *        PRIMITIVE 0000010101
 *         FUNCTION 0000011101
 *             u8[] 0000100101 tag word for u8[] alloced on heap
 */

DECLARE_OBJECT_TYPE(SYMBOL, 3)
DECLARE_OBJECT_TYPE(STRING, 5)
DECLARE_OBJECT_TYPE(NUMBER, 13)
DECLARE_OBJECT_TYPE(PRIMITIVE, 21)
DECLARE_OBJECT_TYPE(FUNCTION, 29)
DECLARE_OBJECT_TYPE(U8VEC, 37)
