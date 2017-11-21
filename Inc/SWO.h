
#ifndef SWO_H_
#define SWO_H_
#ifdef __cplusplus
 extern "C" {
#endif

void SWO_Setup      (void);
void SWO_PrintChar  (char c);
void SWO_PrintString(const char *s);

#ifdef __cplusplus
}
#endif
#endif /* SWO_H_ */
