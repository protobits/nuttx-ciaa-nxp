/****************************************************************************
 * net/uip/uip.h
 *
 *   Copyright (C) 2007-2009, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * This logic was leveraged from uIP which also has a BSD-style license:
 *
 *   Author Adam Dunkels <adam@dunkels.com>
 *   Copyright (c) 2001-2003, Adam Dunkels.
 *   All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef _NET_UIP_UIP_H
#define _NET_UIP_UIP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#ifdef CONFIG_NET

#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <arch/irq.h>
#include <nuttx/net/uip.h>
#include <nuttx/net/arp.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern const uip_ipaddr_t g_alloneaddr;
extern const uip_ipaddr_t g_allzeroaddr;

/* Increasing number used for the IP ID field. */

extern uint16_t g_ipid;

/* Reassembly timer (units: deci-seconds) */

#if UIP_REASSEMBLY && !defined(CONFIG_NET_IPv6)
extern uint8_t uip_reasstmr;
#endif

/* List of applications waiting for ICMP ECHO REPLY */

#if defined(CONFIG_NET_ICMP) && defined(CONFIG_NET_ICMP_PING)
extern struct uip_callback_s *g_echocallback;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Defined in uip_callback.c ************************************************/

void uip_callbackinit(void);
FAR struct uip_callback_s *uip_callbackalloc(struct uip_callback_s **list);
void uip_callbackfree(FAR struct uip_callback_s *cb, struct uip_callback_s **list);
uint16_t uip_callbackexecute(FAR struct uip_driver_s *dev, void *pvconn,
                             uint16_t flags, FAR struct uip_callback_s *list);

#ifdef CONFIG_NET_PKT
/* Defined in uip_pktconn.c *************************************************/

void uip_pktinit(void);
struct uip_pkt_conn *uip_pktalloc(void);
void uip_pktfree(struct uip_pkt_conn *conn);
struct uip_pkt_conn *uip_pktactive(struct uip_eth_hdr *buf);
struct uip_pkt_conn *uip_nextpktconn(struct uip_pkt_conn *conn);

/* Defined in uip_pktcallback.c *********************************************/

uint16_t uip_pktcallback(struct uip_driver_s *dev, struct uip_pkt_conn *conn,
                         uint16_t flags);

/* Defined in uip_pktinput.c ************************************************/

/* Defined in uip_pktpoll.c *************************************************/

void uip_pktpoll(struct uip_driver_s *dev, struct uip_pkt_conn *conn);

#endif /* CONFIG_NET_PKT */

#ifdef CONFIG_NET_UDP
/* Defined in udp_conn.c ****************************************************/

void uip_udpinit(void);
struct uip_udp_conn *uip_udpactive(struct uip_udpip_hdr *buf);
struct uip_udp_conn *uip_nextudpconn(struct uip_udp_conn *conn);

/* Defined in udp_poll.c ****************************************************/

void uip_udppoll(struct uip_driver_s *dev, struct uip_udp_conn *conn);

/* Defined in udp_send.c ****************************************************/

void uip_udpsend(struct uip_driver_s *dev, struct uip_udp_conn *conn);

/* Defined in udp_input.c ***************************************************/

int uip_udpinput(struct uip_driver_s *dev);

/* Defined in udp_callback.c ************************************************/

uint16_t uip_udpcallback(struct uip_driver_s *dev,
                         struct uip_udp_conn *conn, uint16_t flags);
#endif /* CONFIG_NET_UDP */

#ifdef CONFIG_NET_ICMP
/* Defined in icmp_input.c **************************************************/

void uip_icmpinput(struct uip_driver_s *dev);

#ifdef CONFIG_NET_ICMP_PING
/* Defined in icmp_poll.c ***************************************************/

void uip_icmppoll(struct uip_driver_s *dev);

/* Defined in icmp_send.c ***************************************************/

void uip_icmpsend(struct uip_driver_s *dev, uip_ipaddr_t *destaddr);

#endif /* CONFIG_NET_ICMP_PING */
#endif /* CONFIG_NET_ICMP */

#ifdef CONFIG_NET_IGMP
/* Defined in igmp_init.c ***************************************************/

void uip_igmpinit(void);

/* Defined in igmp_input.c **************************************************/

void uip_igmpinput(struct uip_driver_s *dev);

/* Defined in igmp_group.c **************************************************/

void uip_grpinit(void);
FAR struct igmp_group_s *uip_grpalloc(FAR struct uip_driver_s *dev,
                                      FAR const uip_ipaddr_t *addr);
FAR struct igmp_group_s *uip_grpfind(FAR struct uip_driver_s *dev,
                                     FAR const uip_ipaddr_t *addr);
FAR struct igmp_group_s *uip_grpallocfind(FAR struct uip_driver_s *dev,
                                          FAR const uip_ipaddr_t *addr);
void uip_grpfree(FAR struct uip_driver_s *dev,
                 FAR struct igmp_group_s *group);

/* Defined in igmp_msg.c ****************************************************/

void uip_igmpschedmsg(FAR struct igmp_group_s *group, uint8_t msgid);
void uip_igmpwaitmsg(FAR struct igmp_group_s *group, uint8_t msgid);

/* Defined in igmp_poll.c ***************************************************/

void uip_igmppoll(FAR struct uip_driver_s *dev);

/* Defined in igmp_send.c ***************************************************/

void uip_igmpsend(FAR struct uip_driver_s *dev, FAR struct igmp_group_s *group,
                  FAR uip_ipaddr_t *dest);

/* Defined in igmp_timer.c **************************************************/

int uip_decisec2tick(int decisecs);
void uip_igmpstartticks(FAR struct igmp_group_s *group, int ticks);
void uip_igmpstarttimer(FAR struct igmp_group_s *group, uint8_t decisecs);
bool uip_igmpcmptimer(FAR struct igmp_group_s *group, int maxticks);

/* Defined in igmp_mcastmac *****************************************************/

void uip_addmcastmac(FAR struct uip_driver_s *dev, FAR uip_ipaddr_t *ip);
void uip_removemcastmac(FAR struct uip_driver_s *dev, FAR uip_ipaddr_t *ip);

#endif /* CONFIG_NET_IGMP */

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_NET */
#endif /* _NET_UIP_UIP_H */