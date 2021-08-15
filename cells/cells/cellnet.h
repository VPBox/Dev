
#ifndef __CELLNET_H
#define __CELLNET_H

#ifdef __cplusplus
extern "C" {
#endif

extern void init_cell_net();
extern void createveth(int index);
extern void vethtons(int pid,int index);
extern void rnameveth(int index);
extern void starttether(int index);
extern void stoptether(int index);
extern void delveth(int index);

#ifdef __cplusplus
}
#endif

#endif /* __CELLNET_H */
