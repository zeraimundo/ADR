/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
//
//       11.1.1.0
// n0 -------------- n1
//    point-to-point
//


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SegundaAtividade");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (25.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (1));
  clientApps.Start (Seconds (10.0));
  clientApps.Stop (Seconds (25.0));

  /*----------------*/

  NodeContainer nodes2;
  nodes2.Create (2);

  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices2;
  devices2 = pointToPoint2.Install (nodes2);

  InternetStackHelper stack2;
  stack2.Install (nodes2);

  Ipv4AddressHelper address2;
  address2.SetBase ("11.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  UdpEchoServerHelper echoServer2 (7);

  ApplicationContainer serverApps2 = echoServer2.Install (nodes2.Get (1));
  serverApps2.Start (Seconds (1.0));
  serverApps2.Stop (Seconds (25.0));

  UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 7);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (0.8)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (512));

  ApplicationContainer clientApps2 = echoClient2.Install (nodes2.Get (0));
  clientApps2.Start (Seconds (5.0));
  clientApps2.Stop (Seconds (25.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
