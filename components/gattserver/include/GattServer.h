#ifndef _GATT_SERVER_H
#define _GATT_SERVER_H

#include <vector>
#include "host/ble_uuid.h"
#include "host/ble_hs.h"
#include "host/ble_gatt.h"

namespace BLE {

    class UUID {
    private:
        ble_uuid_any_t m_uuid;
    public:
        /**
         * Create a 16bit well-known UUID
         */
        UUID(uint16_t value);
        /**
         * Create a 128bit custom UUID
         */
        UUID(uint8_t* value);
        /**
         * Allow an instance of this class to be cast to the target type for the NimBLE API 
         */
        operator ble_uuid_t*();
    };

    class Characteristic {
    private:
        //std::vector<Descriptor> m_descriptors;
        ble_gatt_chr_def m_chr_def;
        BLE::UUID* m_uuid;
    public:
        //void AddDescriptor(Descriptor d);
        Characteristic(BLE::UUID uuid, ble_gatt_chr_flags flags);
        static int AccessHandler(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt* ctxt, void* arg);
        virtual void* GetValue();
        virtual size_t GetValueSize();
        virtual int SetValue(os_mbuf* om);
        virtual ~Characteristic();
        operator ble_gatt_chr_def();
    };

    class Service {
    private:
        std::vector<ble_gatt_chr_def> m_characteristics;
        ble_gatt_svc_def m_svc_def;
        BLE::UUID* m_uuid;
    public:
        void AddCharacteristic(Characteristic* c);
        Service(BLE::UUID uuid, uint8_t type);
        virtual ~Service();
        operator ble_gatt_svc_def();
    };


    class GattServer {
    private:
        std::vector<ble_gatt_svc_def> m_services;
        int GAPEventHandler(ble_gap_event* event, void* arg);
        void Advertise();
    public:
        static GattServer& Instance();
        ~GattServer() {}
        void Run();
        void Init(const char* name);
        void AddService(Service* s);
    protected:
        GattServer() {};
        GattServer(const GattServer& other) {};
        GattServer& operator = (const GattServer&);
    };
} // namespace BLE
#endif // _GATT_SERVER_H